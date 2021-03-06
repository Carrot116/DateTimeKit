/*
 * ReadWritableInterval.h
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

#import "HLReadableInterval.h"


@class HLChronology;
@protocol HLReadablePeriod;
@protocol HLReadableInstant;

/**
 * Writable interface for an interval.
 *
 * @author Stephen Colebourne
 * @author Brian S O'Neill
 * @since 1.0
 */
@protocol HLReadWritableInterval <HLReadableInterval>

/**
 * Sets this interval from two millisecond instants.
 *
 * @param startInstant  the start of the time interval
 * @param endInstant  the start of the time interval
 * @throws IllegalArgumentException if the end is before the start
 */
- (void)setIntervalStartValue:(NSInteger)startInstant 
                     endValue:(NSInteger)endInstant);

/**
 * Sets this interval to be the same as another.
 *
 * @param interval  the interval to copy
 * @throws IllegalArgumentException if the end is before the start
 */
- (void)setInterval:(id<HLReadableInterval>)interval;

/**
 * Sets this interval from two instants.
 *
 * @param startInstant  the start of the time interval
 * @param endInstant  the start of the time interval
 * @throws IllegalArgumentException if the end is before the start
 */
- (void)setIntervalStartInstant:(id<HLReadableInstant>)startInstant
                     endInstant:(id<HLReadableInstant>)endInstant;

//-----------------------------------------------------------------------
/**
 * Sets the chronology of this time interval.
 *
 * @param chrono  the chronology to use, nil means ISO default
 */
- (void)setChronology:(HLChronology*)chrono;

//-----------------------------------------------------------------------
/**
 * Sets the start of this time interval.
 *
 * @param millisInstant  the start of the time interval,
 *  millisecond instant from 1970-01-01T00:00:00Z
 * @throws IllegalArgumentException if the end is before the start
 */
- (void)setStartMillis:(NSInteger)millisInstant;

/**
 * Sets the start of this time interval as an Instant.
 *
 * @param instant  the start of the time interval
 * @throws IllegalArgumentException if the end is before the start
 */
- (void)setStartInstant:(id<HLReadableInstant>)instant;

//-----------------------------------------------------------------------
/** 
 * Sets the end of this time interval.
 *
 * @param millisInstant  the end of the time interval,
 *  millisecond instant from 1970-01-01T00:00:00Z
 * @throws IllegalArgumentException if the end is before the start
 */
- (void)setEndMillis:(NSInteger)millisInstant;

/** 
 * Sets the end of this time interval as an Instant.
 *
 * @param instant  the end of the time interval
 * @throws IllegalArgumentException if the end is before the start
 */
- (void)setEndInstant:(id<HLReadableInstant>)instant;

//-----------------------------------------------------------------------
/**
 * Sets the duration of this time interval, preserving the start instant.
 *
 * @param duration  new duration for interval
 * @throws IllegalArgumentException if the end is before the start
 * @throws ArithmeticException if the end instant exceeds the capacity of a long
 */
- (void)setDurationAfterStart:(id<HLReadableDuration>)duration;

/**
 * Sets the duration of this time interval, preserving the end instant.
 *
 * @param duration  new duration for interval
 * @throws IllegalArgumentException if the end is before the start
 * @throws ArithmeticException if the start instant exceeds the capacity of a long
 */
- (void)setDurationBeforeEnd:(id<HLReadableDuration>)duration;

//-----------------------------------------------------------------------
/**
 * Sets the period of this time interval, preserving the start instant.
 *
 * @param period  new period for interval, nil means zero length
 * @throws IllegalArgumentException if the end is before the start
 * @throws ArithmeticException if the end instant exceeds the capacity of a long
 */
- (void)setPeriodAfterStart:(id<HLReadablePeriod>)period;

/**
 * Sets the period of this time interval, preserving the end instant.
 *
 * @param period  new period for interval, nil means zero length
 * @throws IllegalArgumentException if the end is before the start
 * @throws ArithmeticException if the start instant exceeds the capacity of a long
 */
- (void)setPeriodBeforeEnd:(id<HLReadablePeriod>)period;

@end
