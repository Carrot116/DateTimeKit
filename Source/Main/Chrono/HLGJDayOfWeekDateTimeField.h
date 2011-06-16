/*
 * HLGJDayOfWeekDateTimeField.h
 * 
 * Horologe
 * Copyright (c) 2010 Pilgrimage Software
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

#import "HLPreciseDurationDateTimeField.h"


@interface HLGJDayOfWeekDateTimeField : HLPreciseDurationDateTimeField {

    @private 
    HLBasicChronology* iChronology;
}

/**
 */
- (id)initWithChronology:(HLBasicChronology*)chronology
                    days:(HLDurationField*)days;

/**
 */
- (NSInteger)get:(NSInteger)instant;

/**
 */
- (NSString*)asText:(NSInteger)fieldValue
                 locale:(NSLocale*)locale;

/**
 */
- (NSString*)asShortText:(NSInteger)fieldValue
                  locale:(NSLocale*)locale;

/**
 */
- (NSInteger)convertText:(NSString*)text
                  locale:(NSLocale*)locale;

- (HLDurationField*)rangeDurationField;

/**
 */
- (NSInteger)minimumValue;

/**
 */
- (NSInteger)maximumValue;

/**
 */
- (NSInteger)maximumTextLength:(NSLocale*)locale;

/**
 */
- (NSInteger)maximumShortTextLength:(NSLocale*)locale;

/**
 */
- (id)readResolve;

@end
