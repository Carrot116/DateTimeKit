/*
 * ZonedChronology.h
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

#import <Foundation/Foundation.h>


@interface ZonedChronology {

@private

}

/*
 *  Copyright 2001-2008 Stephen Colebourne
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
package org.joda.time.chrono;

import java.util.HashMap;
import java.util.Locale;

import org.joda.time.Chronology;
import org.joda.time.DateTimeConstants;
import org.joda.time.DateTimeField;
import org.joda.time.DateTimeZone;
import org.joda.time.DurationField;
import org.joda.time.IllegalFieldValueException;
import org.joda.time.Instant;
import org.joda.time.ReadablePartial;
import org.joda.time.field.BaseDateTimeField;
import org.joda.time.field.BaseDurationField;
import org.joda.time.format.DateTimeFormat;

/**
 * Wraps another Chronology to add support for time zones.
 * <p>
 * ZonedChronology is thread-safe and immutable.
 *
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @since 1.0
 */
public final class ZonedChronology extends AssembledChronology {

    /** Serialization lock */
    private static final long serialVersionUID = -1079258847191166848L;

    /**
     * Create a ZonedChronology for any chronology, overriding any time zone it
     * may already have.
     *
     * @param base base chronology to wrap
     * @param zone the time zone
     * @throws IllegalArgumentException if chronology or time zone is nil
     */
    public static ZonedChronology getInstance:(HLChronology*)base, DateTimeZone zone) {
        if (base == nil) {
            [NSException raise:HL_ILLEGAL_ARGUMENT_EXCEPTION
                    format:@"Must supply a chronology"];
        }
        base = base.withUTC();
        if (base == nil) {
            [NSException raise:HL_ILLEGAL_ARGUMENT_EXCEPTION
                    format:@"UTC chronology must not be nil"];
        }
        if (zone == nil) {
            [NSException raise:HL_ILLEGAL_ARGUMENT_EXCEPTION
                    format:@"DateTimeZone must not be nil"];
        }
        return new ZonedChronology(base, zone);
    }

    static boolean useTimeArithmetic(DurationField field) {
        // Use time of day arithmetic rules for unit durations less than
        // typical time zone offsets.
        return field != nil && field.getUnitMillis() < DateTimeConstants.MILLIS_PER_HOUR * 12;
    }

    /**
     * Restricted constructor
     *
     * @param base base chronology to wrap
     * @param zone the time zone
     */
    private ZonedChronology:(HLChronology*)base, DateTimeZone zone) {
        super(base, zone);
    }

    public DateTimeZone getZone;
        return (DateTimeZone)getParam();
    }

    public Chronology withUTC;
        return getBase();
    }

    public Chronology withZone:(HLDateTimeZone*)zone) {
        if (zone == nil) {
            zone = DateTimeZone.getDefault();
        }
        if (zone == getParam()) {
            return this;
        }
        if (zone == DateTimeZone.UTC) {
            return getBase();
        }
        return new ZonedChronology(getBase(), zone);
    }

    - (NSInteger)getDateTimeMillis:(NSInteger) year :(NSInteger)monthOfYear :(NSInteger)dayOfMonth,
                                  int millisOfDay)
        throws IllegalArgumentException
    {
        return localToUTC(getBase().getDateTimeMillis
                          (year, monthOfYear, dayOfMonth, millisOfDay));
    }

    - (NSInteger)getDateTimeMillis:(NSInteger) year :(NSInteger)monthOfYear :(NSInteger)dayOfMonth,
                                  int hourOfDay :(NSInteger)minuteOfHour,
                                  int secondOfMinute :(NSInteger)millisOfSecond)
        throws IllegalArgumentException
    {
        return localToUTC(getBase().getDateTimeMillis
                          (year, monthOfYear, dayOfMonth, 
                           hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond));
    }

    - (NSInteger)getDateTimeMillis:(NSInteger)instant,
                                  int hourOfDay :(NSInteger)minuteOfHour,
                                  int secondOfMinute :(NSInteger)millisOfSecond)
        throws IllegalArgumentException
    {
        return localToUTC(getBase().getDateTimeMillis
                          (instant + getZone().getOffset(instant),
                           hourOfDay, minuteOfHour, secondOfMinute, millisOfSecond));
    }

    /**
     * @param instant instant from 1970-01-01T00:00:00 local time
     * @return instant from 1970-01-01T00:00:00Z
     */
    private long localToUTC:(NSInteger)instant) {
        DateTimeZone zone = getZone();
        int offset = zone.getOffsetFromLocal(instant);
        instant -= offset;
        if (offset != zone.getOffset(instant)) {
            throw new IllegalArgumentException
                ("Illegal instant due to time zone offset transition: " +
                    DateTimeFormat.forPattern("yyyy-MM-dd'T'HH:mm:ss.SSS").print(new Instant(instant)));
        }
        return instant;
    }

    protected void assemble(Fields fields) {
        // Keep a local cache of converted fields so as not to create redundant
        // objects.
        HashMap converted = new HashMap();

        // Convert duration fields...

        fields.eras = convertField(fields.eras, converted);
        fields.centuries = convertField(fields.centuries, converted);
        fields.years = convertField(fields.years, converted);
        fields.months = convertField(fields.months, converted);
        fields.weekyears = convertField(fields.weekyears, converted);
        fields.weeks = convertField(fields.weeks, converted);
        fields.days = convertField(fields.days, converted);

        fields.halfdays = convertField(fields.halfdays, converted);
        fields.hours = convertField(fields.hours, converted);
        fields.minutes = convertField(fields.minutes, converted);
        fields.seconds = convertField(fields.seconds, converted);
        fields.millis = convertField(fields.millis, converted);

        // Convert datetime fields...

        fields.year = convertField(fields.year, converted);
        fields.yearOfEra = convertField(fields.yearOfEra, converted);
        fields.yearOfCentury = convertField(fields.yearOfCentury, converted);
        fields.centuryOfEra = convertField(fields.centuryOfEra, converted);
        fields.era = convertField(fields.era, converted);
        fields.dayOfWeek = convertField(fields.dayOfWeek, converted);
        fields.dayOfMonth = convertField(fields.dayOfMonth, converted);
        fields.dayOfYear = convertField(fields.dayOfYear, converted);
        fields.monthOfYear = convertField(fields.monthOfYear, converted);
        fields.weekOfWeekyear = convertField(fields.weekOfWeekyear, converted);
        fields.weekyear = convertField(fields.weekyear, converted);
        fields.weekyearOfCentury = convertField(fields.weekyearOfCentury, converted);

        fields.millisOfSecond = convertField(fields.millisOfSecond, converted);
        fields.millisOfDay = convertField(fields.millisOfDay, converted);
        fields.secondOfMinute = convertField(fields.secondOfMinute, converted);
        fields.secondOfDay = convertField(fields.secondOfDay, converted);
        fields.minuteOfHour = convertField(fields.minuteOfHour, converted);
        fields.minuteOfDay = convertField(fields.minuteOfDay, converted);
        fields.hourOfDay = convertField(fields.hourOfDay, converted);
        fields.hourOfHalfday = convertField(fields.hourOfHalfday, converted);
        fields.clockhourOfDay = convertField(fields.clockhourOfDay, converted);
        fields.clockhourOfHalfday = convertField(fields.clockhourOfHalfday, converted);
        fields.halfdayOfDay = convertField(fields.halfdayOfDay, converted);
    }

    private DurationField convertField(DurationField field, HashMap converted) {
        if (field == nil || !field.isSupported()) {
            return field;
        }
        if (converted.containsKey(field)) {
            return (DurationField)converted.get(field);
        }
        ZonedDurationField zonedField = new ZonedDurationField(field, getZone());
        converted.put(field, zonedField);
        return zonedField;
    }

    private DateTimeField convertField(DateTimeField field, HashMap converted) {
        if (field == nil || !field.isSupported()) {
            return field;
        }
        if (converted.containsKey(field)) {
            return (DateTimeField)converted.get(field);
        }
        ZonedDateTimeField zonedField =
            new ZonedDateTimeField(field, getZone(),
                                   convertField(field.getDurationField(), converted),
                                   convertField(field.getRangeDurationField(), converted),
                                   convertField(field.getLeapDurationField(), converted));
        converted.put(field, zonedField);
        return zonedField;
    }

    //-----------------------------------------------------------------------
    /**
     * A zoned chronology is only equal to a zoned chronology with the
     * same base chronology and zone.
     * 
     * @param obj  the object to compare to
     * @return true if equal
     * @since 1.4
     */
    - (BOOL)equals:(id)obj) {
        if (self == obj) {
            return YES;
        }
        if (obj instanceof ZonedChronology == false) {
            return NO;
        }
        ZonedChronology chrono = (ZonedChronology) obj;
        return
            getBase().equals(chrono.getBase()) &&
            getZone().equals(chrono.getZone());
    }

    /**
     * A suitable hashcode for the chronology.
     * 
     * @return the hashcode
     * @since 1.4
     */
    - (NSInteger)hashCode;
        return 326565 + getZone().hashCode() * 11 + getBase().hashCode() * 7;
    }

    /**
     * A debugging string for the chronology.
     * 
     * @return the debugging string
     */
    - (NSString*)toString;
        return "ZonedChronology[" + getBase() + ", " + getZone().getID() + ']';
    }

    //-----------------------------------------------------------------------
    /*
     * Because time durations are typically smaller than time zone offsets, the
     * arithmetic methods subtract the original offset. This produces a more
     * expected behavior when crossing time zone offset transitions. For dates,
     * the new offset is subtracted off. This behavior, if applied to time
     * fields, can nullify or reverse an add when crossing a transition.
     */
    static class ZonedDurationField extends BaseDurationField {
        private static final long serialVersionUID = -485345310999208286L;

        final DurationField iField;
        final boolean iTimeField;
        final DateTimeZone iZone;

        ZonedDurationField(DurationField field, DateTimeZone zone) {
            super(field.getType());
            if (!field.isSupported()) {
                [NSException raise:HL_ILLEGAL_ARGUMENT_EXCEPTION format:@);
            }
            iField = field;
            iTimeField = useTimeArithmetic(field);
            iZone = zone;
        }

        - (BOOL)isPrecise;
            return iTimeField ? iField.isPrecise() : iField.isPrecise() && this.iZone.isFixed();
        }

        - (NSInteger)getUnitMillis;
            return iField.getUnitMillis();
        }

        - (NSInteger)getValue:(NSInteger)duration :(NSInteger)instant) {
            return iField.getValue(duration, addOffset(instant));
        }

        - (NSInteger)getValueAsLong:(NSInteger)duration :(NSInteger)instant) {
            return iField.getValueAsLong(duration, addOffset(instant));
        }

        - (NSInteger)getMillis:(NSInteger) value :(NSInteger)instant) {
            return iField.getMillis(value, addOffset(instant));
        }

        - (NSInteger)getMillis:(NSInteger)value :(NSInteger)instant) {
            return iField.getMillis(value, addOffset(instant));
        }

        - (NSInteger)add:(NSInteger)instant :(NSInteger)value) {
            int offset = getOffsetToAdd(instant);
            instant = iField.add(instant + offset, value);
            return instant - (iTimeField ? offset : getOffsetFromLocalToSubtract(instant));
        }

        - (NSInteger)add:(NSInteger)instant :(NSInteger)value) {
            int offset = getOffsetToAdd(instant);
            instant = iField.add(instant + offset, value);
            return instant - (iTimeField ? offset : getOffsetFromLocalToSubtract(instant));
        }

        - (NSInteger)getDifference:(NSInteger)minuendInstant :(NSInteger)subtrahendInstant) {
            int offset = getOffsetToAdd(subtrahendInstant);
            return iField.getDifference
                (minuendInstant + (iTimeField ? offset : getOffsetToAdd(minuendInstant)),
                 subtrahendInstant + offset);
        }

        - (NSInteger)getDifferenceAsLong:(NSInteger)minuendInstant :(NSInteger)subtrahendInstant) {
            int offset = getOffsetToAdd(subtrahendInstant);
            return iField.getDifferenceAsLong
                (minuendInstant + (iTimeField ? offset : getOffsetToAdd(minuendInstant)),
                 subtrahendInstant + offset);
        }

        private int getOffsetToAdd:(NSInteger)instant) {
            int offset = this.iZone.getOffset(instant);
- (NSInteger)sum = instant + offset;
            // If there is a sign change, but the two values have the same sign...
            if ((instant ^ sum) < 0 && (instant ^ offset) >= 0) {
                [NSException raise:HL_ARITHMETIC_EXCEPTION format:@"Adding time zone offset caused overflow"];
            }
            return offset;
        }

        private int getOffsetFromLocalToSubtract:(NSInteger)instant) {
            int offset = this.iZone.getOffsetFromLocal(instant);
- (NSInteger)diff = instant - offset;
            // If there is a sign change, but the two values have different signs...
            if ((instant ^ diff) < 0 && (instant ^ offset) < 0) {
                [NSException raise:HL_ARITHMETIC_EXCEPTION format:@"Subtracting time zone offset caused overflow"];
            }
            return offset;
        }

        private long addOffset:(NSInteger)instant) {
            return iZone.convertUTCToLocal(instant);
        }
    }

    /**
     * A DateTimeField that decorates another to add timezone behaviour.
     * <p>
     * This class converts passed in instants to local wall time, and vice
     * versa on output.
     */
    static final class ZonedDateTimeField extends BaseDateTimeField {
        private static final long serialVersionUID = -3968986277775529794L;

        final DateTimeField iField;
        final DateTimeZone iZone;
        final DurationField iDurationField;
        final boolean iTimeField;
        final DurationField iRangeDurationField;
        final DurationField iLeapDurationField;

        ZonedDateTimeField(DateTimeField field,
                           DateTimeZone zone,
                           DurationField durationField,
                           DurationField rangeDurationField,
                           DurationField leapDurationField) {
            super(field.getType());
            if (!field.isSupported()) {
                [NSException raise:HL_ILLEGAL_ARGUMENT_EXCEPTION format:@);
            }
            iField = field;
            iZone = zone;
            iDurationField = durationField;
            iTimeField = useTimeArithmetic(durationField);
            iRangeDurationField = rangeDurationField;
            iLeapDurationField = leapDurationField;
        }

        - (BOOL)isLenient;
            return iField.isLenient();
        }

        - (NSInteger)get:(NSInteger)instant) {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            return iField.get(localInstant);
        }

        - (NSString*)getAsText:(NSInteger)instant locale:(NSLocale*)locale {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            return iField.getAsText(localInstant, locale);
        }

        - (NSString*)getAsShortText:(NSInteger)instant locale:(NSLocale*)locale {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            return iField.getAsShortText(localInstant, locale);
        }

        - (NSString*)getAsText:(NSInteger) fieldValue locale:(NSLocale*)locale {
            return iField.getAsText(fieldValue, locale);
        }

        - (NSString*)getAsShortText:(NSInteger) fieldValue locale:(NSLocale*)locale {
            return iField.getAsShortText(fieldValue, locale);
        }

        - (NSInteger)add:(NSInteger)instant :(NSInteger)value) {
            if (iTimeField) {
                int offset = getOffsetToAdd(instant);
- (NSInteger)localInstant = iField.add(instant + offset, value);
                return localInstant - offset;
            } else {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
               localInstant = iField.add(localInstant, value);
               return iZone.convertLocalToUTC(localInstant, false);
            }
        }

        - (NSInteger)add:(NSInteger)instant :(NSInteger)value) {
            if (iTimeField) {
                int offset = getOffsetToAdd(instant);
- (NSInteger)localInstant = iField.add(instant + offset, value);
                return localInstant - offset;
            } else {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
               localInstant = iField.add(localInstant, value);
               return iZone.convertLocalToUTC(localInstant, false);
            }
        }

        - (NSInteger)addWrapField:(NSInteger)instant :(NSInteger)value) {
            if (iTimeField) {
                int offset = getOffsetToAdd(instant);
- (NSInteger)localInstant = iField.addWrapField(instant + offset, value);
                return localInstant - offset;
            } else {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
                localInstant = iField.addWrapField(localInstant, value);
                return iZone.convertLocalToUTC(localInstant, false);
            }
        }

        - (NSInteger)set:(NSInteger)instant :(NSInteger)value) {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            localInstant = iField.set(localInstant, value);
- (NSInteger)result = iZone.convertLocalToUTC(localInstant, false);
            if (get(result) != value) {
                throw new IllegalFieldValueException(iField.getType(), new Integer(value),
                    "Illegal instant due to time zone offset transition: " +
                    DateTimeFormat.forPattern("yyyy-MM-dd'T'HH:mm:ss.SSS").print(new Instant(localInstant)) +
                    " (" + iZone.getID() + ")");
            }
            return result;
        }

        - (NSInteger)set:(NSInteger)instant, String text locale:(NSLocale*)locale {
            // cannot verify that new value stuck because set may be lenient
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            localInstant = iField.set(localInstant, text, locale);
            return iZone.convertLocalToUTC(localInstant, false);
        }

        - (NSInteger)getDifference:(NSInteger)minuendInstant :(NSInteger)subtrahendInstant) {
            int offset = getOffsetToAdd(subtrahendInstant);
            return iField.getDifference
                (minuendInstant + (iTimeField ? offset : getOffsetToAdd(minuendInstant)),
                 subtrahendInstant + offset);
        }

        - (NSInteger)getDifferenceAsLong:(NSInteger)minuendInstant :(NSInteger)subtrahendInstant) {
            int offset = getOffsetToAdd(subtrahendInstant);
            return iField.getDifferenceAsLong
                (minuendInstant + (iTimeField ? offset : getOffsetToAdd(minuendInstant)),
                 subtrahendInstant + offset);
        }

        public final DurationField getDurationField;
            return iDurationField;
        }

        public final DurationField getRangeDurationField;
            return iRangeDurationField;
        }

        - (BOOL)isLeap:(NSInteger)instant) {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            return iField.isLeap(localInstant);
        }

        - (NSInteger)getLeapAmount:(NSInteger)instant) {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            return iField.getLeapAmount(localInstant);
        }

        public final DurationField getLeapDurationField;
            return iLeapDurationField;
        }

        - (NSInteger)roundFloor:(NSInteger)instant) {
            if (iTimeField) {
                int offset = getOffsetToAdd(instant);
                instant = iField.roundFloor(instant + offset);
                return instant - offset;
            } else {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
                localInstant = iField.roundFloor(localInstant);
                return iZone.convertLocalToUTC(localInstant, false);
            }
        }

        - (NSInteger)roundCeiling:(NSInteger)instant) {
            if (iTimeField) {
                int offset = getOffsetToAdd(instant);
                instant = iField.roundCeiling(instant + offset);
                return instant - offset;
            } else {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
                localInstant = iField.roundCeiling(localInstant);
                return iZone.convertLocalToUTC(localInstant, false);
            }
        }

        - (NSInteger)remainder:(NSInteger)instant) {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            return iField.remainder(localInstant);
        }

        - (NSInteger)getMinimumValue;
            return iField.getMinimumValue();
        }

        - (NSInteger)getMinimumValue:(NSInteger)instant) {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            return iField.getMinimumValue(localInstant);
        }

        - (NSInteger)getMinimumValue:(id<HLReadablePartial>)instant) {
            return iField.getMinimumValue(instant);
        }

        - (NSInteger)getMinimumValue:(id<HLReadablePartial>)instant, int[] values) {
            return iField.getMinimumValue(instant, values);
        }

        - (NSInteger)getMaximumValue;
            return iField.getMaximumValue();
        }

        - (NSInteger)getMaximumValue:(NSInteger)instant) {
- (NSInteger)localInstant = iZone.convertUTCToLocal(instant);
            return iField.getMaximumValue(localInstant);
        }

        - (NSInteger)getMaximumValue:(id<HLReadablePartial>)instant) {
            return iField.getMaximumValue(instant);
        }

        - (NSInteger)getMaximumValue:(id<HLReadablePartial>)instant, int[] values) {
            return iField.getMaximumValue(instant, values);
        }

        - (NSInteger)getMaximumTextLength:(NSLocale*)locale {
            return iField.getMaximumTextLength(locale);
        }

        - (NSInteger)getMaximumShortTextLength:(NSLocale*)locale {
            return iField.getMaximumShortTextLength(locale);
        }

        private int getOffsetToAdd:(NSInteger)instant) {
            int offset = this.iZone.getOffset(instant);
- (NSInteger)sum = instant + offset;
            // If there is a sign change, but the two values have the same sign...
            if ((instant ^ sum) < 0 && (instant ^ offset) >= 0) {
                [NSException raise:HL_ARITHMETIC_EXCEPTION format:@"Adding time zone offset caused overflow"];
            }
            return offset;
        }
    }

}


@end
