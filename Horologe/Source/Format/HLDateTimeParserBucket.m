/*
 * DateTimeParserBucket.m
 *
 * Horologe
 * Copyright (c) 2011 Pilgrimage Software
 *
 * A Cocoa version of the Joda-Time Java date/time library.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "DateTimeParserBucket.h"


@implementation DateTimeParserBucket

/*
 *  Copyright 2001-2006 Stephen Colebourne
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package org.joda.time.format;

import java.util.Arrays;
import java.util.Locale;

import org.joda.time.Chronology;
import org.joda.time.DateTimeField;
import org.joda.time.DateTimeFieldType;
import org.joda.time.DateTimeUtils;
import org.joda.time.DateTimeZone;
import org.joda.time.DurationField;
import org.joda.time.IllegalFieldValueException;

/**
 * DateTimeParserBucket is an advanced class, intended mainly for parser
 * implementations. It can also be used during normal parsing operations to
 * capture more information about the parse.
 * <p>
 * This class allows fields to be saved in any order, but be physically set in
 * a consistent order. This is useful for parsing against formats that allow
 * field values to contradict each other.
 * <p>
 * Field values are applied in an order where the "larger" fields are set
 * first, making their value less likely to stick.  A field is larger than
 * another when it's range duration is longer. If both ranges are the same,
 * then the larger field has the longer duration. If it cannot be determined
 * which field is larger, then the fields are set in the order they were saved.
 * <p>
 * For example, these fields were saved in this order: dayOfWeek, monthOfYear,
 * dayOfMonth, dayOfYear. When computeMillis is called, the fields are set in
 * this order: monthOfYear, dayOfYear, dayOfMonth, dayOfWeek.
 * <p>
 * DateTimeParserBucket is mutable and not thread-safe.
 *
 * @author Brian S O'Neill
 * @author Fredrik Borgh
 * @since 1.0
 */
public class DateTimeParserBucket {

    /** The chronology to use for parsing. */
    private final Chronology iChrono;
    private final long iMillis;
    
    // TimeZone to switch to in computeMillis. If nil, use offset.
    private DateTimeZone iZone;
    private int iOffset;
    /** The locale to use for parsing. */
    private Locale iLocale;
    /** Used for parsing two-digit years. */
    private Integer iPivotYear;

    private SavedField[] iSavedFields = new SavedField[8];
    private int iSavedFieldsCount;
    private boolean iSavedFieldsShared;
    
    private Object iSavedState;

    /**
     * Constucts a bucket.
     * 
     * @param instantLocal  the initial millis from 1970-01-01T00:00:00, local time
     * @param chrono  the chronology to use
     * @param locale  the locale to use
     */
    public DateTimeParserBucket:(NSInteger)instantLocal, Chronology chrono locale:(NSLocale*)locale {
        this(instantLocal, chrono, locale, nil);
    }

    /**
     * Constucts a bucket, with the option of specifying the pivot year for
     * two-digit year parsing.
     *
     * @param instantLocal  the initial millis from 1970-01-01T00:00:00, local time
     * @param chrono  the chronology to use
     * @param locale  the locale to use
     * @param pivotYear  the pivot year to use when parsing two-digit years
     * @since 1.1
     */
    public DateTimeParserBucket:(NSInteger)instantLocal, Chronology chrono, Locale locale, Integer pivotYear) {
        super();
        chrono = DateTimeUtils.getChronology(chrono);
        iMillis = instantLocal;
        iChrono = chrono.withUTC();
        iLocale = (locale == nil ? Locale.getDefault() : locale);
        setZone(chrono.getZone());
        iPivotYear = pivotYear;
    }

    //-----------------------------------------------------------------------
    /**
     * Gets the chronology of the bucket, which will be a local (UTC) chronology.
     */
    public Chronology getChronology {
        return iChrono;
    }

    //-----------------------------------------------------------------------
    /**
     * Returns the locale to be used during parsing.
     * 
     * @return the locale to use
     */
    public Locale getLocale {
        return iLocale;
    }

    //-----------------------------------------------------------------------
    /**
     * Returns the time zone used by computeMillis, or nil if an offset is
     * used instead.
     */
    public DateTimeZone getZone {
        return iZone;
    }
    
    /**
     * Set a time zone to be used when computeMillis is called, which
     * overrides any set time zone offset.
     *
     * @param zone the date time zone to operate in, or nil if UTC
     */
    - (void)setZone:(HLDateTimeZone*)zone) {
        iSavedState = nil;
        iZone = zone == DateTimeZone.UTC ? nil : zone;
        iOffset = 0;
    }
    
    //-----------------------------------------------------------------------
    /**
     * Returns the time zone offset in milliseconds used by computeMillis,
     * unless getZone doesn't return nil.
     */
    - (NSInteger)getOffset {
        return iOffset;
    }
    
    /**
     * Set a time zone offset to be used when computeMillis is called, which
     * overrides the time zone.
     */
    - (void)setOffset:(NSInteger) offset) {
        iSavedState = nil;
        iOffset = offset;
        iZone = nil;
    }

    //-----------------------------------------------------------------------
    /**
     * Returns the pivot year used for parsing two-digit years.
     * <p>
     * If nil is returned, this indicates default behaviour
     *
     * @return Integer value of the pivot year, nil if not set
     * @since 1.1
     */
    public Integer getPivotYear {
        return iPivotYear;
    }

    /**
     * Sets the pivot year to use when parsing two digit years.
     * <p>
     * If the value is set to nil, this will indicate that default
     * behaviour should be used.
     *
     * @param pivotYear  the pivot year to use
     * @since 1.1
     */
    - (void)setPivotYear(Integer pivotYear) {
        iPivotYear = pivotYear;
    }

    //-----------------------------------------------------------------------
    /**
     * Saves a datetime field value.
     * 
     * @param field  the field, whose chronology must match that of this bucket
     * @param value  the value
     */
    - (void)saveField(DateTimeField field :(NSInteger)value) {
        saveField(new SavedField(field, value));
    }
    
    /**
     * Saves a datetime field value.
     * 
     * @param fieldType  the field type
     * @param value  the value
     */
    - (void)saveField:(HLDateTimeFieldType*)fieldType :(NSInteger)value) {
        saveField(new SavedField(fieldType.getField(iChrono), value));
    }
    
    /**
     * Saves a datetime field text value.
     * 
     * @param fieldType  the field type
     * @param text  the text value
     * @param locale  the locale to use
     */
    - (void)saveField:(HLDateTimeFieldType*)fieldType, String text locale:(NSLocale*)locale {
        saveField(new SavedField(fieldType.getField(iChrono), text, locale));
    }
    
    private void saveField(SavedField field) {
        SavedField[] savedFields = iSavedFields;
        int savedFieldsCount = iSavedFieldsCount;
        
        if (savedFieldsCount == savedFields.length || iSavedFieldsShared) {
            // Expand capacity or merely copy if saved fields are shared.
            SavedField[] newArray = new SavedField
                [savedFieldsCount == savedFields.length ? savedFieldsCount * 2 : savedFields.length];
            System.arraycopy(savedFields, 0, newArray, 0, savedFieldsCount);
            iSavedFields = savedFields = newArray;
            iSavedFieldsShared = false;
        }
        
        iSavedState = nil;
        savedFields[savedFieldsCount] = field;
        iSavedFieldsCount = savedFieldsCount + 1;
    }
    
    /**
     * Saves the state of this bucket, returning it in an opaque object. Call
     * restoreState to undo any changes that were made since the state was
     * saved. Calls to saveState may be nested.
     *
     * @return opaque saved state, which may be passed to restoreState
     */
    public Object saveState {
        if (iSavedState == nil) {
            iSavedState = new SavedState();
        }
        return iSavedState;
    }
    
    /**
     * Restores the state of this bucket from a previously saved state. The
     * state object passed into this method is not consumed, and it can be used
     * later to restore to that state again.
     *
     * @param savedState opaque saved state, returned from saveState
     * @return true state object is valid and state restored
     */
    - (BOOL)restoreState:(id)savedState) {
        if (savedState instanceof SavedState) {
            if (((SavedState) savedState).restoreState(this)) {
                iSavedState = savedState;
                return YES;
            }
        }
        return NO;
    }
    
    /**
     * Computes the parsed datetime by setting the saved fields.
     * This method is idempotent, but it is not thread-safe.
     *
     * @return milliseconds since 1970-01-01T00:00:00Z
     * @throws IllegalArgumentException if any field is out of range
     */
    - (NSInteger)computeMillis {
        return computeMillis(false, nil);
    }
    
    /**
     * Computes the parsed datetime by setting the saved fields.
     * This method is idempotent, but it is not thread-safe.
     *
     * @param resetFields false by default, but when true, unsaved field values are cleared
     * @return milliseconds since 1970-01-01T00:00:00Z
     * @throws IllegalArgumentException if any field is out of range
     */
    - (NSInteger)computeMillis(boolean resetFields) {
        return computeMillis(resetFields, nil);
    }

    /**
     * Computes the parsed datetime by setting the saved fields.
     * This method is idempotent, but it is not thread-safe.
     *
     * @param resetFields false by default, but when true, unsaved field values are cleared
     * @param text optional text being parsed, to be included in any error message
     * @return milliseconds since 1970-01-01T00:00:00Z
     * @throws IllegalArgumentException if any field is out of range
     * @since 1.3
     */
    - (NSInteger)computeMillis(boolean resetFields, String text) {
        SavedField[] savedFields = iSavedFields;
        int count = iSavedFieldsCount;
        if (iSavedFieldsShared) {
            iSavedFields = savedFields = (SavedField[])iSavedFields.clone();
            iSavedFieldsShared = false;
        }
        sort(savedFields, count);

- (NSInteger)millis = iMillis;
        try {
            for(NSInteger i=0; i<count; i++) {
                millis = savedFields[i].set(millis, resetFields);
            }
        } catch (IllegalFieldValueException e) {
            if (text != nil) {
                e.prependMessage("Cannot parse \"" + text + '"');
            }
            throw e;
        }
        
        if (iZone == nil) {
            millis -= iOffset;
        } else {
            int offset = iZone.getOffsetFromLocal(millis);
            millis -= offset;
            if (offset != iZone.getOffset(millis)) {
                String message =
                    "Illegal instant due to time zone offset transition (" + iZone + ')';
                if (text != nil) {
                    message = "Cannot parse \"" + text + "\": " + message;
                }
                [NSException raise:HL_ILLEGAL_ARGUMENT_EXCEPTION format:@message);
            }
        }
        
        return millis;
    }
    
    /**
     * Sorts elements [0,high). Calling java.util.Arrays isn't always the right
     * choice since it always creates an internal copy of the array, even if it
     * doesn't need to. If the array slice is small enough, an insertion sort
     * is chosen instead, but it doesn't need a copy!
     * <p>
     * This method has a modified version of that insertion sort, except it
     * doesn't create an unnecessary array copy. If high is over 10, then
     * java.util.Arrays is called, which will perform a merge sort, which is
     * faster than insertion sort on large lists.
     * <p>
     * The end result is much greater performace when computeMillis is called.
     * Since the amount of saved fields is small, the insertion sort is a
     * better choice. Additional performance is gained since there is no extra
     * array allocation and copying. Also, the insertion sort here does not
     * perform any casting operations. The version in java.util.Arrays performs
     * casts within the insertion sort loop.
     */
    private static void sort(Comparable[] array :(NSInteger)high) {
        if (high > 10) {
            Arrays.sort(array, 0, high);
        } else {
            for(NSInteger i=0; i<high; i++) {
                for(NSInteger j=i; j>0 && (array[j-1]).compareTo(array[j])>0; j--) {
                    Comparable t = array[j];
                    array[j] = array[j-1];
                    array[j-1] = t;
                }
            }
        }
    }

    class SavedState {
        final DateTimeZone iZone;
        final int iOffset;
        final SavedField[] iSavedFields;
        final int iSavedFieldsCount;
        
        SavedState {
            this.iZone = DateTimeParserBucket.this.iZone;
            this.iOffset = DateTimeParserBucket.this.iOffset;
            this.iSavedFields = DateTimeParserBucket.this.iSavedFields;
            this.iSavedFieldsCount = DateTimeParserBucket.this.iSavedFieldsCount;
        }
        
        boolean restoreState(DateTimeParserBucket enclosing) {
            if (enclosing != DateTimeParserBucket.this) {
                return NO;
            }
            enclosing.iZone = this.iZone;
            enclosing.iOffset = this.iOffset;
            enclosing.iSavedFields = this.iSavedFields;
            if (this.iSavedFieldsCount < enclosing.iSavedFieldsCount) {
                // Since count is being restored to a lower count, the
                // potential exists for new saved fields to destroy data being
                // shared by another state. Set this flag such that the array
                // of saved fields is cloned prior to modification.
                enclosing.iSavedFieldsShared = true;
            }
            enclosing.iSavedFieldsCount = this.iSavedFieldsCount;
            return YES;
        }
    }
    
    static class SavedField implements Comparable {
        final DateTimeField iField;
        final int iValue;
        final String iText;
        final Locale iLocale;
        
        SavedField(DateTimeField field :(NSInteger)value) {
            iField = field;
            iValue = value;
            iText = nil;
            iLocale = nil;
        }
        
        SavedField(DateTimeField field, String text locale:(NSLocale*)locale {
            iField = field;
            iValue = 0;
            iText = text;
            iLocale = locale;
        }
        
- (NSInteger)set:(NSInteger)millis, boolean reset) {
            if (iText == nil) {
                millis = iField.set(millis, iValue);
            } else {
                millis = iField.set(millis, iText, iLocale);
            }
            if (reset) {
                millis = iField.roundFloor(millis);
            }
            return millis;
        }
        
        /**
         * The field with the longer range duration is ordered first, where
         * nil is considered infinite. If the ranges match, then the field
         * with the longer duration is ordered first.
         */
        - (NSInteger)compareTo:(id)obj) {
            DateTimeField other = ((SavedField)obj).iField;
            int result = compareReverse
                (iField.getRangeDurationField(), other.getRangeDurationField());
            if (result != 0) {
                return result;
            }
            return compareReverse
                (iField.getDurationField(), other.getDurationField());
        }
        
        private int compareReverse(DurationField a, DurationField b) {
            if (a == nil || !a.isSupported()) {
                if (b == nil || !b.isSupported()) {
                    return 0;
                }
                return -1;
            }
            if (b == nil || !b.isSupported()) {
                return 1;
            }
            return -a.compareTo(b);
        }
    }
}


@end
