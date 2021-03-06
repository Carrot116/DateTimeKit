/*
 * Instant.h
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

#import "HLAbstractInstant.h"
#import "HLReadableInstant.h"


@class HLMutableDateTime;
@class HLDateTime;
@class HLChronology;
@protocol HLReadableDuration;

/**
 * Instant is the standard implementation of a fully immutable instant in time.
 * <p>
 * <code>Instant</code> is an implementation of {@link ReadableInstant}.
 * As with all instants, it represents an exact point on the time-line,
 * but limited to the precision of milliseconds. An <code>Instant</code>
 * should be used to represent a point in time irrespective of any other
 * factor, such as chronology or time zone.
 * <p>
 * Internally, the class holds one piece of data, the instant as milliseconds
 * from the Java epoch of 1970-01-01T00:00:00Z.
 * <p>
 * For example, an Instant can be used to compare two <code>DateTime</code>
 * objects irrespective of chronology or time zone.
 * <pre>
 * boolean sameInstant = dt1.toInstant().equals(dt2.toInstant());
 * </pre>
 * Note that the following code will also perform the same check:
 * <pre>
 * boolean sameInstant = dt1.isEqual(dt2);
 * </pre>
 * <p>
 * Instant is thread-safe and immutable.
 *
 * @author Stephen Colebourne
 * @since 1.0
 */
@interface HLInstant : HLAbstractInstant <HLReadableInstant> {
    
@private
    /** The millis from 1970-01-01T00:00:00Z */
    NSInteger _iMillis;
    
}

//-----------------------------------------------------------------------
/**
 * Constructs an instance set to the milliseconds from 1970-01-01T00:00:00Z.
 * 
 * @param instant  the milliseconds from 1970-01-01T00:00:00Z
 */
- (id)initWithInstantValue:(NSInteger)instant;

/**
 * Constructs an instance from an Object that represents a datetime.
 * <p>
 * The recognised object types are defined in {@link ConverterManager} and
 * include String, Calendar and Date.
 *
 * @param instant  the datetime object, nil means now
 * @throws IllegalArgumentException if the instant is invalid
 */
- (id)initWithInstant:(id)instant;

//-----------------------------------------------------------------------
/**
 * Get this object as an Instant by returning <code>this</code>.
 * 
 * @return <code>this</code>
 */
- (HLInstant*)toInstant;

//-----------------------------------------------------------------------
/**
 * Gets a copy of this instant with different millis.
 * <p>
 * The returned object will be either be a new Instant or <code>this</code>.
 *
 * @param newMillis  the new millis, from 1970-01-01T00:00:00Z
 * @return a copy of this instant with different millis
 */
- (HLInstant*)withMillis:(NSInteger)newMillis;

/**
 * Gets a copy of this instant with the specified duration added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 * 
 * @param durationToAdd  the duration to add to this one
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this instant with the duration added
 * @throws ArithmeticException if the new instant exceeds the capacity of a long
 */
- (HLInstant*)withDurationValueAdded:(NSInteger)durationToAdd 
                    usingScalar:(NSInteger)scalar;

/**
 * Gets a copy of this instant with the specified duration added.
 * <p>
 * If the addition is zero, then <code>this</code> is returned.
 * 
 * @param durationToAdd  the duration to add to this one, nil means zero
 * @param scalar  the amount of times to add, such as -1 to subtract once
 * @return a copy of this instant with the duration added
 * @throws ArithmeticException if the new instant exceeds the capacity of a long
 */
- (HLInstant*)withDurationAdded:(id<HLReadableDuration>)durationToAdd 
                    usingScalar:(NSInteger)scalar;

//-----------------------------------------------------------------------
/**
 * Gets a copy of this instant with the specified duration added.
 * <p>
 * If the amount is zero or nil, then <code>this</code> is returned.
 * 
 * @param duration  the duration to add to this one
 * @return a copy of this instant with the duration added
 * @throws ArithmeticException if the new instant exceeds the capacity of a long
 */
- (HLInstant*)plusDurationValue:(NSInteger)duration;

/**
 * Gets a copy of this instant with the specified duration added.
 * <p>
 * If the amount is zero or nil, then <code>this</code> is returned.
 * 
 * @param duration  the duration to add to this one, nil means zero
 * @return a copy of this instant with the duration added
 * @throws ArithmeticException if the new instant exceeds the capacity of a long
 */
- (HLInstant*)plusDuration:(id<HLReadableDuration>)duration;

//-----------------------------------------------------------------------
/**
 * Gets a copy of this instant with the specified duration taken away.
 * <p>
 * If the amount is zero or nil, then <code>this</code> is returned.
 * 
 * @param duration  the duration to reduce this instant by
 * @return a copy of this instant with the duration taken away
 * @throws ArithmeticException if the new instant exceeds the capacity of a long
 */
- (HLInstant*)minusDurationValue:(NSInteger)duration;

/**
 * Gets a copy of this instant with the specified duration taken away.
 * <p>
 * If the amount is zero or nil, then <code>this</code> is returned.
 * 
 * @param duration  the duration to reduce this instant by
 * @return a copy of this instant with the duration taken away
 * @throws ArithmeticException if the new instant exceeds the capacity of a long
 */
- (HLInstant*)minusDuration:(id<HLReadableDuration>)duration;

//-----------------------------------------------------------------------
/**
 * Gets the milliseconds of the instant.
 * 
 * @return the number of milliseconds since 1970-01-01T00:00:00Z
 */
- (NSInteger)millis;

/**
 * Gets the chronology of the instant, which is ISO in the UTC zone.
 * <p>
 * This method returns {@link ISOChronology#getInstanceUTC()} which
 * corresponds to the definition of the Java epoch 1970-01-01T00:00:00Z.
 * 
 * @return ISO in the UTC zone
 */
- (HLChronology*)chronology;

//-----------------------------------------------------------------------
/**
 * Get this object as a DateTime using ISOChronology in the default zone.
 * <p>
 * This method returns a DateTime object in the default zone.
 * This differs from the similarly named method on DateTime, DateMidnight
 * or MutableDateTime which retains the time zone. The difference is
 * because Instant really represents a time <i>without</i> a zone,
 * thus calling this method we really have no zone to 'retain' and
 * hence expect to switch to the default zone.
 * <p>
 * This method definition preserves compatability with earlier versions
 * of Joda-Time.
 *
 * @return a DateTime using the same millis
 */
- (HLDateTime*)toDateTime;

/**
 * Get this object as a DateTime using ISOChronology in the default zone.
 * This method is identical to <code>toDateTime()</code>.
 * <p>
 * This method returns a DateTime object in the default zone.
 * This differs from the similarly named method on DateTime, DateMidnight
 * or MutableDateTime which retains the time zone. The difference is
 * because Instant really represents a time <i>without</i> a zone,
 * thus calling this method we really have no zone to 'retain' and
 * hence expect to switch to the default zone.
 * <p>
 * This method definition preserves compatability with earlier versions
 * of Joda-Time.
 *
 * @return a DateTime using the same millis with ISOChronology
 * @deprecated Use toDateTime() as it is identical
 */
- (HLDateTime*)toDateTimeISO;

/**
 * Get this object as a MutableDateTime using ISOChronology in the default zone.
 * <p>
 * This method returns a MutableDateTime object in the default zone.
 * This differs from the similarly named method on DateTime, DateMidnight
 * or MutableDateTime which retains the time zone. The difference is
 * because Instant really represents a time <i>without</i> a zone,
 * thus calling this method we really have no zone to 'retain' and
 * hence expect to switch to the default zone.
 * <p>
 * This method definition preserves compatability with earlier versions
 * of Joda-Time.
 *
 * @return a MutableDateTime using the same millis
 */
- (HLMutableDateTime*)toMutableDateTime;

/**
 * Get this object as a MutableDateTime using ISOChronology in the default zone.
 * This method is identical to <code>toMutableDateTime()</code>.
 * <p>
 * This method returns a MutableDateTime object in the default zone.
 * This differs from the similarly named method on DateTime, DateMidnight
 * or MutableDateTime which retains the time zone. The difference is
 * because Instant really represents a time <i>without</i> a zone,
 * thus calling this method we really have no zone to 'retain' and
 * hence expect to switch to the default zone.
 * <p>
 * This method definition preserves compatability with earlier versions
 * of Joda-Time.
 *
 * @return a MutableDateTime using the same millis with ISOChronology
 * @deprecated Use toMutableDateTime() as it is identical
 */
- (HLMutableDateTime*)toMutableDateTimeISO;

@end
