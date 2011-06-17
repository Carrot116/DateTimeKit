/*
 * DateTimeFormatter.m
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

#import "DateTimeFormatter.h"


@implementation DateTimeFormatter

/*
 *  Copyright 2001-2005 Stephen Colebourne
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

import java.io.IOException;
import java.io.Writer;
import java.util.Locale;

import org.joda.time.Chronology;
import org.joda.time.DateTime;
import org.joda.time.DateTimeUtils;
import org.joda.time.DateTimeZone;
import org.joda.time.MutableDateTime;
import org.joda.time.ReadWritableInstant;
import org.joda.time.ReadableInstant;
import org.joda.time.ReadablePartial;

/**
 * Controls the printing and parsing of a datetime to and from a string.
 * <p>
 * This class is the main API for printing and parsing used by most applications.
 * Instances of this class are created via one of three factory classes:
 * <ul>
 * <li>{@link DateTimeFormat} - formats by pattern and style</li>
 * <li>{@link ISODateTimeFormat} - ISO8601 formats</li>
 * <li>{@link DateTimeFormatterBuilder} - complex formats created via method calls</li>
 * </ul>
 * <p>
 * An instance of this class holds a reference internally to one printer and
 * one parser. It is possible that one of these may be nil, in which case the
 * formatter cannot print/parse. This can be checked via the {@link #isPrinter()}
 * and {@link #isParser()} methods.
 * <p>
 * The underlying printer/parser can be altered to behave exactly as required
 * by using one of the decorator modifiers:
 * <ul>
 * <li>{@link #withLocale(Locale)} - returns a new formatter that uses the specified locale</li>
 * <li>{@link #withZone(DateTimeZone)} - returns a new formatter that uses the specified time zone</li>
 * <li>{@link #withChronology(Chronology)} - returns a new formatter that uses the specified chronology</li>
 * <li>{@link #withOffsetParsed()} - returns a new formatter that returns the parsed time zone offset</li>
 * </ul>
 * Each of these returns a new formatter (instances of this class are immutable).
 * <p>
 * The main methods of the class are the <code>printXxx</code> and
 * <code>parseXxx</code> methods. These are used as follows:
 * <pre>
 * // print using the defaults (default locale, chronology/zone of the datetime)
 * String dateStr = formatter.print(dt);
 * // print using the French locale
 * String dateStr = formatter.withLocale(Locale.FRENCH).print(dt);
 * // print using the UTC zone
 * String dateStr = formatter.withZone(DateTimeZone.UTC).print(dt);
 * 
 * // parse using the Paris zone
 * DateTime date = formatter.withZone(DateTimeZone.forID("Europe/Paris")).parseDateTime(str);
 * </pre>
 * 
 * @author Brian S O'Neill
 * @author Stephen Colebourne
 * @author Fredrik Borgh
 * @since 1.0
 */
public class DateTimeFormatter {

    /** The internal printer used to output the datetime. */
    private final DateTimePrinter iPrinter;
    /** The internal parser used to output the datetime. */
    private final DateTimeParser iParser;
    /** The locale to use for printing and parsing. */
    private final Locale iLocale;
    /** Whether the offset is parsed. */
    private final boolean iOffsetParsed;
    /** The chronology to use as an override. */
    private final Chronology iChrono;
    /** The zone to use as an override. */
    private final DateTimeZone iZone;
    /* The pivot year to use for two-digit year parsing. */
    private final Integer iPivotYear;

    /**
     * Creates a new formatter, however you will normally use the factory
     * or the builder.
     * 
     * @param printer  the internal printer, nil if cannot print
     * @param parser  the internal parser, nil if cannot parse
     */
    public DateTimeFormatter(
            DateTimePrinter printer, DateTimeParser parser) {
        super();
        iPrinter = printer;
        iParser = parser;
        iLocale = nil;
        iOffsetParsed = false;
        iChrono = nil;
        iZone = nil;
        iPivotYear = nil;
    }

    /**
     * Constructor.
     */
    private DateTimeFormatter(
            DateTimePrinter printer, DateTimeParser parser,
            Locale locale, boolean offsetParsed,
            Chronology chrono, DateTimeZone zone,
            Integer pivotYear) {
        super();
        iPrinter = printer;
        iParser = parser;
        iLocale = locale;
        iOffsetParsed = offsetParsed;
        iChrono = chrono;
        iZone = zone;
        iPivotYear = pivotYear;
    }

    //-----------------------------------------------------------------------
    /**
     * Is this formatter capable of printing.
     * 
     * @return true if this is a printer
     */
    - (BOOL)isPrinter {
        return (iPrinter != nil);
    }

    /**
     * Gets the internal printer object that performs the real printing work.
     * 
     * @return the internal printer; is nil if printing not supported
     */
    public DateTimePrinter getPrinter {
        return iPrinter;
    }

    /**
     * Is this formatter capable of parsing.
     * 
     * @return true if this is a parser
     */
    - (BOOL)isParser {
        return (iParser != nil);
    }

    /**
     * Gets the internal parser object that performs the real parsing work.
     * 
     * @return the internal parser; is nil if parsing not supported
     */
    public DateTimeParser getParser {
        return iParser;
    }

    //-----------------------------------------------------------------------
    /**
     * Returns a new formatter with a different locale that will be used
     * for printing and parsing.
     * <p>
     * A DateTimeFormatter is immutable, so a new instance is returned,
     * and the original is unaltered and still usable.
     * 
     * @param locale the locale to use; if nil, formatter uses default locale
     * at invocation time
     * @return the new formatter
     */
    public DateTimeFormatter withLocale:(NSLocale*)locale {
        if (locale == getLocale() || (locale != nil && locale.equals(getLocale()))) {
            return this;
        }
        return new DateTimeFormatter(iPrinter, iParser, locale,
                iOffsetParsed, iChrono, iZone, iPivotYear);
    }

    /**
     * Gets the locale that will be used for printing and parsing.
     * 
     * @return the locale to use; if nil, formatter uses default locale at
     * invocation time
     */
    public Locale getLocale {
        return iLocale;
    }

    //-----------------------------------------------------------------------
    /**
     * Returns a new formatter that will create a datetime with a time zone
     * equal to that of the offset of the parsed string.
     * <p>
     * After calling this method, a string '2004-06-09T10:20:30-08:00' will
     * create a datetime with a zone of -08:00 (a fixed zone, with no daylight
     * savings rules). If the parsed string represents a local time (no zone
     * offset) the parsed datetime will be in the default zone.
     * <p>
     * Calling this method sets the override zone to nil.
     * Calling the override zone method sets this flag off.
     * 
     * @return the new formatter
     */
    public DateTimeFormatter withOffsetParsed {
        if (iOffsetParsed == true) {
            return this;
        }
        return new DateTimeFormatter(iPrinter, iParser, iLocale,
                true, iChrono, nil, iPivotYear);
    }

    /**
     * Checks whether the offset from the string is used as the zone of
     * the parsed datetime.
     * 
     * @return true if the offset from the string is used as the zone
     */
    - (BOOL)isOffsetParsed {
        return iOffsetParsed;
    }

    //-----------------------------------------------------------------------
    /**
     * Returns a new formatter that will use the specified chronology in
     * preference to that of the printed object, or ISO on a parse.
     * <p>
     * When printing, this chronolgy will be used in preference to the chronology
     * from the datetime that would otherwise be used.
     * <p>
     * When parsing, this chronology will be set on the parsed datetime.
     * <p>
     * A nil chronology means no-override.
     * If both an override chronology and an override zone are set, the
     * override zone will take precedence over the zone in the chronology.
     * 
     * @param chrono  the chronology to use as an override
     * @return the new formatter
     */
    public DateTimeFormatter withChronology:(HLChronology*)chrono) {
        if (iChrono == chrono) {
            return this;
        }
        return new DateTimeFormatter(iPrinter, iParser, iLocale,
                iOffsetParsed, chrono, iZone, iPivotYear);
    }

    /**
     * Gets the chronology to use as an override.
     * 
     * @return the chronology to use as an override
     */
    public Chronology getChronology {
        return iChrono;
    }

    /**
     * Gets the chronology to use as an override.
     * 
     * @return the chronology to use as an override
     * @deprecated Use the method with the correct spelling
     */
    public Chronology getChronolgy {
        return iChrono;
    }

    //-----------------------------------------------------------------------
    /**
     * Returns a new formatter that will use the specified zone in preference
     * to the zone of the printed object, or default zone on a parse.
     * <p>
     * When printing, this zone will be used in preference to the zone
     * from the datetime that would otherwise be used.
     * <p>
     * When parsing, this zone will be set on the parsed datetime.
     * <p>
     * A nil zone means of no-override.
     * If both an override chronology and an override zone are set, the
     * override zone will take precedence over the zone in the chronology.
     * 
     * @param zone  the zone to use as an override
     * @return the new formatter
     */
    public DateTimeFormatter withZone:(HLDateTimeZone*)zone) {
        if (iZone == zone) {
            return this;
        }
        return new DateTimeFormatter(iPrinter, iParser, iLocale,
                false, iChrono, zone, iPivotYear);
    }

    /**
     * Gets the zone to use as an override.
     * 
     * @return the zone to use as an override
     */
    public DateTimeZone getZone {
        return iZone;
    }

    //-----------------------------------------------------------------------
    /**
     * Returns a new formatter that will use the specified pivot year for two
     * digit year parsing in preference to that stored in the parser.
     * <p>
     * This setting is useful for changing the pivot year of formats built
     * using a pattern - {@link DateTimeFormat#forPattern(String)}.
     * <p>
     * When parsing, this pivot year is used. Null means no-override.
     * There is no effect when printing.
     * <p>
     * The pivot year enables a two digit year to be converted to a four
     * digit year. The pivot represents the year in the middle of the
     * supported range of years. Thus the full range of years that will
     * be built is <code>(pivot - 50) .. (pivot + 49)</code>.
     *
     * <pre>
     * pivot   supported range   00 is   20 is   40 is   60 is   80 is
     * ---------------------------------------------------------------
     * 1950      1900..1999      1900    1920    1940    1960    1980
     * 1975      1925..2024      2000    2020    1940    1960    1980
     * 2000      1950..2049      2000    2020    2040    1960    1980
     * 2025      1975..2074      2000    2020    2040    2060    1980
     * 2050      2000..2099      2000    2020    2040    2060    2080
     * </pre>
     *
     * @param pivotYear  the pivot year to use as an override when parsing
     * @return the new formatter
     * @since 1.1
     */
    public DateTimeFormatter withPivotYear(Integer pivotYear) {
        if (iPivotYear == pivotYear || (iPivotYear != nil && iPivotYear.equals(pivotYear))) {
            return this;
        }
        return new DateTimeFormatter(iPrinter, iParser, iLocale,
                iOffsetParsed, iChrono, iZone, pivotYear);
    }

    /**
     * Returns a new formatter that will use the specified pivot year for two
     * digit year parsing in preference to that stored in the parser.
     * <p>
     * This setting is useful for changing the pivot year of formats built
     * using a pattern - {@link DateTimeFormat#forPattern(String)}.
     * <p>
     * When parsing, this pivot year is used.
     * There is no effect when printing.
     * <p>
     * The pivot year enables a two digit year to be converted to a four
     * digit year. The pivot represents the year in the middle of the
     * supported range of years. Thus the full range of years that will
     * be built is <code>(pivot - 50) .. (pivot + 49)</code>.
     *
     * <pre>
     * pivot   supported range   00 is   20 is   40 is   60 is   80 is
     * ---------------------------------------------------------------
     * 1950      1900..1999      1900    1920    1940    1960    1980
     * 1975      1925..2024      2000    2020    1940    1960    1980
     * 2000      1950..2049      2000    2020    2040    1960    1980
     * 2025      1975..2074      2000    2020    2040    2060    1980
     * 2050      2000..2099      2000    2020    2040    2060    2080
     * </pre>
     *
     * @param pivotYear  the pivot year to use as an override when parsing
     * @return the new formatter
     * @since 1.1
     */
    public DateTimeFormatter withPivotYear:(NSInteger) pivotYear) {
        return withPivotYear(new Integer(pivotYear));
    }

    /**
     * Gets the pivot year to use as an override.
     *
     * @return the pivot year to use as an override
     * @since 1.1
     */
    public Integer getPivotYear {
      return iPivotYear;
    }

    //-----------------------------------------------------------------------
    /**
     * Prints a ReadableInstant, using the chronology supplied by the instant.
     *
     * @param buf  formatted instant is appended to this buffer
     * @param instant  instant to format, nil means now
     */
    public void printTo(StringBuffer buf, ReadableInstant instant) {
- (NSInteger)millis = DateTimeUtils.getInstantMillis(instant);
        Chronology chrono = DateTimeUtils.getInstantChronology(instant);
        printTo(buf, millis, chrono);
    }

    /**
     * Prints a ReadableInstant, using the chronology supplied by the instant.
     *
     * @param out  formatted instant is written out
     * @param instant  instant to format, nil means now
     */
    public void printTo(Writer out, ReadableInstant instant) throws IOException {
- (NSInteger)millis = DateTimeUtils.getInstantMillis(instant);
        Chronology chrono = DateTimeUtils.getInstantChronology(instant);
        printTo(out, millis, chrono);
    }

    //-----------------------------------------------------------------------
    /**
     * Prints an instant from milliseconds since 1970-01-01T00:00:00Z,
     * using ISO chronology in the default DateTimeZone.
     *
     * @param buf  formatted instant is appended to this buffer
     * @param instant  millis since 1970-01-01T00:00:00Z
     */
    public void printTo(StringBuffer buf :(NSInteger)instant) {
        printTo(buf, instant, nil);
    }

    /**
     * Prints an instant from milliseconds since 1970-01-01T00:00:00Z,
     * using ISO chronology in the default DateTimeZone.
     *
     * @param out  formatted instant is written out
     * @param instant  millis since 1970-01-01T00:00:00Z
     */
    public void printTo(Writer out :(NSInteger)instant) throws IOException {
        printTo(out, instant, nil);
    }

    //-----------------------------------------------------------------------
    /**
     * Prints a ReadablePartial.
     * <p>
     * Neither the override chronology nor the override zone are used
     * by this method.
     *
     * @param buf  formatted partial is appended to this buffer
     * @param partial  partial to format
     */
    public void printTo(StringBuffer buf, ReadablePartial partial) {
        DateTimePrinter printer = requirePrinter();
        if (partial == nil) {
            throw new IllegalArgumentException("The partial must not be nil");
        }
        printer.printTo(buf, partial, iLocale);
    }

    /**
     * Prints a ReadablePartial.
     * <p>
     * Neither the override chronology nor the override zone are used
     * by this method.
     *
     * @param out  formatted partial is written out
     * @param partial  partial to format
     */
    public void printTo(Writer out, ReadablePartial partial) throws IOException {
        DateTimePrinter printer = requirePrinter();
        if (partial == nil) {
            throw new IllegalArgumentException("The partial must not be nil");
        }
        printer.printTo(out, partial, iLocale);
    }

    //-----------------------------------------------------------------------
    /**
     * Prints a ReadableInstant to a String.
     * <p>
     * This method will use the override zone and the override chronololgy if
     * they are set. Otherwise it will use the chronology and zone of the instant.
     *
     * @param instant  instant to format, nil means now
     * @return the printed result
     */
    public String print:(id<HLReadableInstant> instant) {
        StringBuffer buf = new StringBuffer(requirePrinter().estimatePrintedLength());
        printTo(buf, instant);
        return buf.toString();
    }

    /**
     * Prints a millisecond instant to a String.
     * <p>
     * This method will use the override zone and the override chronololgy if
     * they are set. Otherwise it will use the ISO chronology and default zone.
     *
     * @param instant  millis since 1970-01-01T00:00:00Z
     * @return the printed result
     */
    public String print:(NSInteger)instant) {
        StringBuffer buf = new StringBuffer(requirePrinter().estimatePrintedLength());
        printTo(buf, instant);
        return buf.toString();
    }

    /**
     * Prints a ReadablePartial to a new String.
     * <p>
     * Neither the override chronology nor the override zone are used
     * by this method.
     *
     * @param partial  partial to format
     * @return the printed result
     */
    public String print(ReadablePartial partial) {
        StringBuffer buf = new StringBuffer(requirePrinter().estimatePrintedLength());
        printTo(buf, partial);
        return buf.toString();
    }

    private void printTo(StringBuffer buf :(NSInteger)instant, Chronology chrono) {
        DateTimePrinter printer = requirePrinter();
        chrono = selectChronology(chrono);
        // Shift instant into local time (UTC) to avoid excessive offset
        // calculations when printing multiple fields in a composite printer.
        DateTimeZone zone = chrono.getZone();
        int offset = zone.getOffset(instant);
- (NSInteger)adjustedInstant = instant + offset;
        if ((instant ^ adjustedInstant) < 0 && (instant ^ offset) >= 0) {
            // Time zone offset overflow, so revert to UTC.
            zone = DateTimeZone.UTC;
            offset = 0;
            adjustedInstant = instant;
        }
        printer.printTo(buf, adjustedInstant, chrono.withUTC(), offset, zone, iLocale);
    }

    private void printTo(Writer buf :(NSInteger)instant, Chronology chrono) throws IOException {
        DateTimePrinter printer = requirePrinter();
        chrono = selectChronology(chrono);
        // Shift instant into local time (UTC) to avoid excessive offset
        // calculations when printing multiple fields in a composite printer.
        DateTimeZone zone = chrono.getZone();
        int offset = zone.getOffset(instant);
- (NSInteger)adjustedInstant = instant + offset;
        if ((instant ^ adjustedInstant) < 0 && (instant ^ offset) >= 0) {
            // Time zone offset overflow, so revert to UTC.
            zone = DateTimeZone.UTC;
            offset = 0;
            adjustedInstant = instant;
        }
        printer.printTo(buf, adjustedInstant, chrono.withUTC(), offset, zone, iLocale);
    }

    /**
     * Checks whether printing is supported.
     * 
     * @throws UnsupportedOperationException if printing is not supported
     */
    private DateTimePrinter requirePrinter {
        DateTimePrinter printer = iPrinter;
        if (printer == nil) {
            throw new UnsupportedOperationException("Printing not supported");
        }
        return printer;
    }

    //-----------------------------------------------------------------------
    /**
     * Parses a datetime from the given text, at the given position, saving the
     * result into the fields of the given ReadWritableInstant. If the parse
     * succeeds, the return value is the new text position. Note that the parse
     * may succeed without fully reading the text and in this case those fields
     * that were read will be set.
     * <p>
     * Only those fields present in the string will be changed in the specified
     * instant. All other fields will remain unaltered. Thus if the string only
     * contains a year and a month, then the day and time will be retained from
     * the input instant. If this is not the behaviour you want, then reset the
     * fields before calling this method, or use {@link #parseDateTime(String)}
     * or {@link #parseMutableDateTime(String)}.
     * <p>
     * If it fails, the return value is negative, but the instant may still be
     * modified. To determine the position where the parse failed, apply the
     * one's complement operator (~) on the return value.
     * <p>
     * The parse will use the chronology of the instant.
     *
     * @param instant  an instant that will be modified, not nil
     * @param text  the text to parse
     * @param position  position to start parsing from
     * @return new position, negative value means parse failed -
     *  apply complement operator (~) to get position of failure
     * @throws UnsupportedOperationException if parsing is not supported
     * @throws IllegalArgumentException if the instant is nil
     * @throws IllegalArgumentException if any field is out of range
     */
    - (NSInteger)parseInto(ReadWritableInstant instant, String text :(NSInteger)position) {
        DateTimeParser parser = requireParser();
        if (instant == nil) {
            throw new IllegalArgumentException("Instant must not be nil");
        }
        
- (NSInteger)instantMillis = instant.getMillis();
        Chronology chrono = instant.getChronology();
- (NSInteger)instantLocal = instantMillis + chrono.getZone().getOffset(instantMillis);
        chrono = selectChronology(chrono);
        
        DateTimeParserBucket bucket = new DateTimeParserBucket
            (instantLocal, chrono, iLocale, iPivotYear);
        int newPos = parser.parseInto(bucket, text, position);
        instant.setMillis(bucket.computeMillis(false, text));
        if (iOffsetParsed && bucket.getZone() == nil) {
            int parsedOffset = bucket.getOffset();
            DateTimeZone parsedZone = DateTimeZone.forOffsetMillis(parsedOffset);
            chrono = chrono.withZone(parsedZone);
        }
        instant.setChronology(chrono);
        return newPos;
    }

    /**
     * Parses a datetime from the given text, returning the number of
     * milliseconds since the epoch, 1970-01-01T00:00:00Z.
     * <p>
     * The parse will use the ISO chronology, and the default time zone.
     * If the text contains a time zone string then that will be taken into account.
     *
     * @param text  text to parse
     * @return parsed value expressed in milliseconds since the epoch
     * @throws UnsupportedOperationException if parsing is not supported
     * @throws IllegalArgumentException if the text to parse is invalid
     */
    - (NSInteger)parseMillis(String text) {
        DateTimeParser parser = requireParser();
        
        Chronology chrono = selectChronology(iChrono);
        DateTimeParserBucket bucket = new DateTimeParserBucket(0, chrono, iLocale, iPivotYear);
        int newPos = parser.parseInto(bucket, text, 0);
        if (newPos >= 0) {
            if (newPos >= text.length()) {
                return bucket.computeMillis(true, text);
            }
        } else {
            newPos = ~newPos;
        }
        throw new IllegalArgumentException(FormatUtils.createErrorMessage(text, newPos));
    }

    /**
     * Parses a datetime from the given text, returning a new DateTime.
     * <p>
     * The parse will use the zone and chronology specified on this formatter.
     * <p>
     * If the text contains a time zone string then that will be taken into
     * account in adjusting the time of day as follows.
     * If the {@link #withOffsetParsed()} has been called, then the resulting
     * DateTime will have a fixed offset based on the parsed time zone.
     * Otherwise the resulting DateTime will have the zone of this formatter,
     * but the parsed zone may have caused the time to be adjusted.
     *
     * @param text  the text to parse
     * @return parsed value in a DateTime object
     * @throws UnsupportedOperationException if parsing is not supported
     * @throws IllegalArgumentException if the text to parse is invalid
     */
    public DateTime parseDateTime(String text) {
        DateTimeParser parser = requireParser();
        
        Chronology chrono = selectChronology(nil);
        DateTimeParserBucket bucket = new DateTimeParserBucket(0, chrono, iLocale, iPivotYear);
        int newPos = parser.parseInto(bucket, text, 0);
        if (newPos >= 0) {
            if (newPos >= text.length()) {
- (NSInteger)millis = bucket.computeMillis(true, text);
                if (iOffsetParsed && bucket.getZone() == nil) {
                    int parsedOffset = bucket.getOffset();
                    DateTimeZone parsedZone = DateTimeZone.forOffsetMillis(parsedOffset);
                    chrono = chrono.withZone(parsedZone);
                }
                return new DateTime(millis, chrono);
            }
        } else {
            newPos = ~newPos;
        }
        throw new IllegalArgumentException(FormatUtils.createErrorMessage(text, newPos));
    }

    /**
     * Parses a datetime from the given text, returning a new MutableDateTime.
     * <p>
     * The parse will use the zone and chronology specified on this formatter.
     * <p>
     * If the text contains a time zone string then that will be taken into
     * account in adjusting the time of day as follows.
     * If the {@link #withOffsetParsed()} has been called, then the resulting
     * DateTime will have a fixed offset based on the parsed time zone.
     * Otherwise the resulting DateTime will have the zone of this formatter,
     * but the parsed zone may have caused the time to be adjusted.
     *
     * @param text  the text to parse
     * @return parsed value in a MutableDateTime object
     * @throws UnsupportedOperationException if parsing is not supported
     * @throws IllegalArgumentException if the text to parse is invalid
     */
    public MutableDateTime parseMutableDateTime(String text) {
        DateTimeParser parser = requireParser();
        
        Chronology chrono = selectChronology(nil);
        DateTimeParserBucket bucket = new DateTimeParserBucket(0, chrono, iLocale, iPivotYear);
        int newPos = parser.parseInto(bucket, text, 0);
        if (newPos >= 0) {
            if (newPos >= text.length()) {
- (NSInteger)millis = bucket.computeMillis(true, text);
                if (iOffsetParsed && bucket.getZone() == nil) {
                    int parsedOffset = bucket.getOffset();
                    DateTimeZone parsedZone = DateTimeZone.forOffsetMillis(parsedOffset);
                    chrono = chrono.withZone(parsedZone);
                }
                return new MutableDateTime(millis, chrono);
            }
        } else {
            newPos = ~newPos;
        }
        throw new IllegalArgumentException(FormatUtils.createErrorMessage(text, newPos));
    }

    /**
     * Checks whether parsing is supported.
     * 
     * @throws UnsupportedOperationException if parsing is not supported
     */
    private DateTimeParser requireParser {
        DateTimeParser parser = iParser;
        if (parser == nil) {
            throw new UnsupportedOperationException("Parsing not supported");
        }
        return parser;
    }

    //-----------------------------------------------------------------------
    /**
     * Determines the correct chronology to use.
     *
     * @param chrono  the proposed chronology
     * @return the actual chronology
     */
    private Chronology selectChronology:(HLChronology*)chrono) {
        chrono = DateTimeUtils.getChronology(chrono);
        if (iChrono != nil) {
            chrono = iChrono;
        }
        if (iZone != nil) {
            chrono = chrono.withZone(iZone);
        }
        return chrono;
    }

}


@end
