/*
 *  Project: user_kernel_interface - File: jiffies.c
 *  Copyright (C) 2019 - Tania Hagn - tania@df9ry.de
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uki/jiffies.h"
#include "uki/bug.h"
#include "uki/time64.h"

unsigned long volatile __jiffy_data jiffies = INITIAL_JIFFIES;

/*
 * Convert jiffies to milliseconds and back.
 *
 * Avoid unnecessary multiplications/divisions in the
 * two most common HZ cases:
 */
unsigned int jiffies_to_msecs(const unsigned long j)
{
#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
	return (MSEC_PER_SEC / HZ) * j;
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
	return (j + (HZ / MSEC_PER_SEC) - 1)/(HZ / MSEC_PER_SEC);
#else
# if BITS_PER_LONG == 32
	return (HZ_TO_MSEC_MUL32 * j + (1ULL << HZ_TO_MSEC_SHR32) - 1) >>
	       HZ_TO_MSEC_SHR32;
# else
	return DIV_ROUND_UP(j * HZ_TO_MSEC_NUM, HZ_TO_MSEC_DEN);
# endif
#endif
}

unsigned int jiffies_to_usecs(const unsigned long j)
{
	/*
	 * Hz usually doesn't go much further MSEC_PER_SEC.
	 * jiffies_to_usecs() and usecs_to_jiffies() depend on that.
	 */
	BUG_ON(HZ > USEC_PER_SEC);

#if !(USEC_PER_SEC % HZ)
	return (USEC_PER_SEC / HZ) * j;
#else
# if BITS_PER_LONG == 32
	return (HZ_TO_USEC_MUL32 * j) >> HZ_TO_USEC_SHR32;
# else
	return (j * HZ_TO_USEC_NUM) / HZ_TO_USEC_DEN;
# endif
#endif
}

/*
 * mktime64 - Converts date to seconds.
 * Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 * Assumes input in normal date format, i.e. 1980-12-31 23:59:59
 * => year=1980, mon=12, day=31, hour=23, min=59, sec=59.
 *
 * [For the Julian calendar (which was used in Russia before 1917,
 * Britain & colonies before 1752, anywhere else before 1582,
 * and is still in use by some communities) leave out the
 * -year/100+year/400 terms, and add 10.]
 *
 * This algorithm was first published by Gauss (I think).
 *
 * A leap second can be indicated by calling this function with sec as
 * 60 (allowable under ISO 8601).  The leap second is treated the same
 * as the following second since they don't exist in UNIX time.
 *
 * An encoding of midnight at the end of the day as 24:00:00 - ie. midnight
 * tomorrow - (allowable under ISO 8601) is supported.
 */
time64_t mktime64(const unsigned int year0, const unsigned int mon0,
		const unsigned int day, const unsigned int hour,
		const unsigned int min, const unsigned int sec)
{
	unsigned int mon = mon0, year = year0;

	/* 1..12 -> 11,12,1..10 */
	if (0 >= (int) (mon -= 2)) {
		mon += 12;	/* Puts Feb last since it has leap day */
		year -= 1;
	}

	return ((((time64_t)
		  (year/4 - year/100 + year/400 + 367*mon/12 + day) +
		  year*365 - 719499
	    )*24 + hour /* now have hours - midnight tomorrow handled here */
	  )*60 + min /* now have minutes */
	)*60 + sec; /* finally seconds */
}

/**
 * ns_to_timespec - Convert nanoseconds to timespec
 * @nsec:       the nanoseconds value to be converted
 *
 * Returns the timespec representation of the nsec parameter.
 */
struct timespec ns_to_timespec(const s64 nsec)
{
	struct timespec ts;
	s32 rem;

	if (!nsec)
		return (struct timespec) {0, 0};

	ts.tv_sec = div_s64_rem(nsec, NSEC_PER_SEC, &rem);
	if (rem < 0) {
		ts.tv_sec--;
		rem += NSEC_PER_SEC;
	}
	ts.tv_nsec = rem;

	return ts;
}

#if 0
struct timeval ns_to_timeval(const s64 nsec)
{
	struct timespec ts = ns_to_timespec(nsec);
	struct timeval tv;

	tv.tv_sec = ts.tv_sec;
	tv.tv_usec = (suseconds_t) ts.tv_nsec / 1000;

	return tv;
}
#endif

#if 0
struct __kernel_old_timeval ns_to_kernel_old_timeval(const s64 nsec)
{
	struct timespec64 ts = ns_to_timespec64(nsec);
	struct __kernel_old_timeval tv;

	tv.tv_sec = ts.tv_sec;
	tv.tv_usec = (suseconds_t)ts.tv_nsec / 1000;

	return tv;
}
#endif

/**
 * set_normalized_timespec - set timespec sec and nsec parts and normalize
 *
 * @ts:		pointer to timespec variable to be set
 * @sec:	seconds to set
 * @nsec:	nanoseconds to set
 *
 * Set seconds and nanoseconds field of a timespec variable and
 * normalize to the timespec storage format
 *
 * Note: The tv_nsec part is always in the range of
 *	0 <= tv_nsec < NSEC_PER_SEC
 * For negative values only the tv_sec field is negative !
 */
void set_normalized_timespec64(struct timespec64 *ts, time64_t sec, s64 nsec)
{
	while (nsec >= NSEC_PER_SEC) {
		/*
		 * The following asm() prevents the compiler from
		 * optimising this loop into a modulo operation. See
		 * also __iter_div_u64_rem() in include/linux/time.h
		 */
		asm("" : "+rm"(nsec));
		nsec -= NSEC_PER_SEC;
		++sec;
	}
	while (nsec < 0) {
		asm("" : "+rm"(nsec));
		nsec += NSEC_PER_SEC;
		--sec;
	}
	ts->tv_sec = sec;
	ts->tv_nsec = nsec;
}

/**
 * ns_to_timespec64 - Convert nanoseconds to timespec64
 * @nsec:       the nanoseconds value to be converted
 *
 * Returns the timespec64 representation of the nsec parameter.
 */
struct timespec64 ns_to_timespec64(const s64 nsec)
{
	struct timespec64 ts;
	s32 rem;

	if (!nsec)
		return (struct timespec64) {0, 0};

	ts.tv_sec = div_s64_rem(nsec, NSEC_PER_SEC, &rem);
	if (rem < 0) {
		ts.tv_sec--;
		rem += NSEC_PER_SEC;
	}
	ts.tv_nsec = rem;

	return ts;
}

/**
 * msecs_to_jiffies: - convert milliseconds to jiffies
 * @m:	time in milliseconds
 *
 * conversion is done as follows:
 *
 * - negative values mean 'infinite timeout' (MAX_JIFFY_OFFSET)
 *
 * - 'too large' values [that would result in larger than
 *   MAX_JIFFY_OFFSET values] mean 'infinite timeout' too.
 *
 * - all other values are converted to jiffies by either multiplying
 *   the input value by a factor or dividing it with a factor and
 *   handling any 32-bit overflows.
 *   for the details see __msecs_to_jiffies()
 *
 * msecs_to_jiffies() checks for the passed in value being a constant
 * via __builtin_constant_p() allowing gcc to eliminate most of the
 * code, __msecs_to_jiffies() is called if the value passed does not
 * allow constant folding and the actual conversion must be done at
 * runtime.
 * the _msecs_to_jiffies helpers are the HZ dependent conversion
 * routines found in include/linux/jiffies.h
 */
unsigned long __msecs_to_jiffies(const unsigned int m)
{
	/*
	 * Negative value, means infinite timeout:
	 */
	if ((int)m < 0)
		return MAX_JIFFY_OFFSET;
	return _msecs_to_jiffies(m);
}

unsigned long __usecs_to_jiffies(const unsigned int u)
{
	if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
		return MAX_JIFFY_OFFSET;
	return _usecs_to_jiffies(u);
}

/*
 * The TICK_NSEC - 1 rounds up the value to the next resolution.  Note
 * that a remainder subtract here would not do the right thing as the
 * resolution values don't fall on second boundries.  I.e. the line:
 * nsec -= nsec % TICK_NSEC; is NOT a correct resolution rounding.
 * Note that due to the small error in the multiplier here, this
 * rounding is incorrect for sufficiently large values of tv_nsec, but
 * well formed timespecs should have tv_nsec < NSEC_PER_SEC, so we're
 * OK.
 *
 * Rather, we just shift the bits off the right.
 *
 * The >> (NSEC_JIFFIE_SC - SEC_JIFFIE_SC) converts the scaled nsec
 * value to a scaled second value.
 */
static unsigned long
__timespec64_to_jiffies(u64 sec, long nsec)
{
	nsec = nsec + TICK_NSEC - 1;

	if (sec >= MAX_SEC_IN_JIFFIES){
		sec = MAX_SEC_IN_JIFFIES;
		nsec = 0;
	}
	return ((sec * SEC_CONVERSION) +
		(((u64)nsec * NSEC_CONVERSION) >>
		 (NSEC_JIFFIE_SC - SEC_JIFFIE_SC))) >> SEC_JIFFIE_SC;

}

static unsigned long
__timespec_to_jiffies(unsigned long sec, long nsec)
{
	return __timespec64_to_jiffies((u64)sec, nsec);
}

unsigned long
timespec64_to_jiffies(const struct timespec64 *value)
{
	return __timespec64_to_jiffies(value->tv_sec, value->tv_nsec);
}

void
jiffies_to_timespec64(const unsigned long jiffies, struct timespec64 *value)
{
	/*
	 * Convert jiffies to nanoseconds and separate with
	 * one divide.
	 */
	u32 rem;
	value->tv_sec = div_u64_rem((u64)jiffies * TICK_NSEC,
				    NSEC_PER_SEC, &rem);
	value->tv_nsec = rem;
}

/*
 * We could use a similar algorithm to timespec_to_jiffies (with a
 * different multiplier for usec instead of nsec). But this has a
 * problem with rounding: we can't exactly add TICK_NSEC - 1 to the
 * usec value, since it's not necessarily integral.
 *
 * We could instead round in the intermediate scaled representation
 * (i.e. in units of 1/2^(large scale) jiffies) but that's also
 * perilous: the scaling introduces a small positive error, which
 * combined with a division-rounding-upward (i.e. adding 2^(scale) - 1
 * units to the intermediate before shifting) leads to accidental
 * overflow and overestimates.
 *
 * At the cost of one additional multiplication by a constant, just
 * use the timespec implementation.
 */
unsigned long
timeval_to_jiffies(const struct timeval *value)
{
	return __timespec_to_jiffies(value->tv_sec,
				     value->tv_usec * NSEC_PER_USEC);
}

void jiffies_to_timeval(const unsigned long jiffies, struct timeval *value)
{
	/*
	 * Convert jiffies to nanoseconds and separate with
	 * one divide.
	 */
	u32 rem;

	value->tv_sec = div_u64_rem((u64)jiffies * TICK_NSEC,
				    NSEC_PER_SEC, &rem);
	value->tv_usec = rem / NSEC_PER_USEC;
}

/*
 * Convert jiffies/jiffies_64 to clock_t and back.
 */
clock_t jiffies_to_clock_t(unsigned long x)
{
#if ((TICK_NSEC % (NSEC_PER_SEC / USER_HZ)) == 0)
# if HZ < USER_HZ
	return x * (USER_HZ / HZ);
# else
	return x / (HZ / USER_HZ);
# endif
#else
	return div_u64((u64)x * TICK_NSEC, NSEC_PER_SEC / USER_HZ);
#endif
}

unsigned long clock_t_to_jiffies(unsigned long x)
{
#if (HZ % USER_HZ)==0
	if (x >= ~0UL / (HZ / USER_HZ))
		return ~0UL;
	return x * (HZ / USER_HZ);
#else
	/* Don't worry about loss of precision here .. */
	if (x >= ~0UL / HZ * USER_HZ)
		return ~0UL;

	/* .. but do try to contain it here */
	return div_u64((u64)x * HZ, USER_HZ);
#endif
}

u64 jiffies_64_to_clock_t(u64 x)
{
#if (TICK_NSEC % (NSEC_PER_SEC / USER_HZ)) == 0
# if HZ < USER_HZ
	x = div_u64(x * USER_HZ, HZ);
# elif HZ > USER_HZ
	x = div_u64(x, HZ / USER_HZ);
# else
	/* Nothing to do */
# endif
#else
	/*
	 * There are better ways that don't overflow early,
	 * but even this doesn't overflow in hundreds of years
	 * in 64 bits, so..
	 */
	x = div_u64(x * TICK_NSEC, (NSEC_PER_SEC / USER_HZ));
#endif
	return x;
}

u64 nsec_to_clock_t(u64 x)
{
#if (NSEC_PER_SEC % USER_HZ) == 0
	return div_u64(x, NSEC_PER_SEC / USER_HZ);
#elif (USER_HZ % 512) == 0
	return div_u64(x * USER_HZ / 512, NSEC_PER_SEC / 512);
#else
	/*
         * max relative error 5.7e-8 (1.8s per year) for USER_HZ <= 1024,
         * overflow after 64.99 years.
         * exact for HZ=60, 72, 90, 120, 144, 180, 300, 600, 900, ...
         */
	return div_u64(x * 9, (9ull * NSEC_PER_SEC + (USER_HZ / 2)) / USER_HZ);
#endif
}

u64 jiffies64_to_nsecs(u64 j)
{
#if !(NSEC_PER_SEC % HZ)
	return (NSEC_PER_SEC / HZ) * j;
# else
	return div_u64(j * HZ_TO_NSEC_NUM, HZ_TO_NSEC_DEN);
#endif
}

/**
 * nsecs_to_jiffies64 - Convert nsecs in u64 to jiffies64
 *
 * @n:	nsecs in u64
 *
 * Unlike {m,u}secs_to_jiffies, type of input is not unsigned int but u64.
 * And this doesn't return MAX_JIFFY_OFFSET since this function is designed
 * for scheduler, not for use in device drivers to calculate timeout value.
 *
 * note:
 *   NSEC_PER_SEC = 10^9 = (5^9 * 2^9) = (1953125 * 512)
 *   ULLONG_MAX ns = 18446744073.709551615 secs = about 584 years
 */
u64 nsecs_to_jiffies64(u64 n)
{
#if (NSEC_PER_SEC % HZ) == 0
	/* Common case, HZ = 100, 128, 200, 250, 256, 500, 512, 1000 etc. */
	return div_u64(n, NSEC_PER_SEC / HZ);
#elif (HZ % 512) == 0
	/* overflow after 292 years if HZ = 1024 */
	return div_u64(n * HZ / 512, NSEC_PER_SEC / 512);
#else
	/*
	 * Generic case - optimized for cases where HZ is a multiple of 3.
	 * overflow after 64.99 years, exact for HZ = 60, 72, 90, 120 etc.
	 */
	return div_u64(n * 9, (9ull * NSEC_PER_SEC + HZ / 2) / HZ);
#endif
}

/**
 * nsecs_to_jiffies - Convert nsecs in u64 to jiffies
 *
 * @n:	nsecs in u64
 *
 * Unlike {m,u}secs_to_jiffies, type of input is not unsigned int but u64.
 * And this doesn't return MAX_JIFFY_OFFSET since this function is designed
 * for scheduler, not for use in device drivers to calculate timeout value.
 *
 * note:
 *   NSEC_PER_SEC = 10^9 = (5^9 * 2^9) = (1953125 * 512)
 *   ULLONG_MAX ns = 18446744073.709551615 secs = about 584 years
 */
unsigned long nsecs_to_jiffies(u64 n)
{
	return (unsigned long)nsecs_to_jiffies64(n);
}

/*
 * Add two timespec64 values and do a safety check for overflow.
 * It's assumed that both values are valid (>= 0).
 * And, each timespec64 is in normalized form.
 */
struct timespec64 timespec64_add_safe(const struct timespec64 lhs,
				const struct timespec64 rhs)
{
	struct timespec64 res;

	set_normalized_timespec64(&res, (timeu64_t) lhs.tv_sec + rhs.tv_sec,
			lhs.tv_nsec + rhs.tv_nsec);

	if (res.tv_sec < lhs.tv_sec || res.tv_sec < rhs.tv_sec) {
		res.tv_sec = TIME64_MAX;
		res.tv_nsec = 0;
	}

	return res;
}

#if 0
int get_timespec64(struct timespec64 *ts,
		   const struct __user *uts)
{
	struct __kernel_timespec kts;
	int ret;

	ret = copy_from_user(&kts, uts, sizeof(kts));
	if (ret)
		return -EFAULT;

	ts->tv_sec = kts.tv_sec;

	/* Zero out the padding for 32 bit systems or in compat mode */
	if (IS_ENABLED(CONFIG_64BIT_TIME) && in_compat_syscall())
		kts.tv_nsec &= 0xFFFFFFFFUL;

	ts->tv_nsec = kts.tv_nsec;

	return 0;
}
#endif

#if 0
int put_timespec64(const struct timespec64 *ts,
		   struct __kernel_timespec __user *uts)
{
	struct __kernel_timespec kts = {
		.tv_sec = ts->tv_sec,
		.tv_nsec = ts->tv_nsec
	};

	return copy_to_user(uts, &kts, sizeof(kts)) ? -EFAULT : 0;
}
#endif

#if 0
static int __get_old_timespec32(struct timespec64 *ts64,
				   const struct old_timespec32 __user *cts)
{
	struct old_timespec32 ts;
	int ret;

	ret = copy_from_user(&ts, cts, sizeof(ts));
	if (ret)
		return -EFAULT;

	ts64->tv_sec = ts.tv_sec;
	ts64->tv_nsec = ts.tv_nsec;

	return 0;
}
#endif

#if 0
static int __put_old_timespec32(const struct timespec64 *ts64,
				   struct old_timespec32 __user *cts)
{
	struct old_timespec32 ts = {
		.tv_sec = ts64->tv_sec,
		.tv_nsec = ts64->tv_nsec
	};
	return copy_to_user(cts, &ts, sizeof(ts)) ? -EFAULT : 0;
}
#endif

#if 0
int get_old_timespec32(struct timespec64 *ts, const void __user *uts)
{
	if (COMPAT_USE_64BIT_TIME)
		return copy_from_user(ts, uts, sizeof(*ts)) ? -EFAULT : 0;
	else
		return __get_old_timespec32(ts, uts);
}
#endif

#if 0
int put_old_timespec32(const struct timespec64 *ts, void __user *uts)
{
	if (COMPAT_USE_64BIT_TIME)
		return copy_to_user(uts, ts, sizeof(*ts)) ? -EFAULT : 0;
	else
		return __put_old_timespec32(ts, uts);
}
#endif

#if 0
int get_itimerspec64(struct itimerspec64 *it,
			const struct __kernel_itimerspec __user *uit)
{
	int ret;

	ret = get_timespec64(&it->it_interval, &uit->it_interval);
	if (ret)
		return ret;

	ret = get_timespec64(&it->it_value, &uit->it_value);

	return ret;
}
#endif

#if 0
int put_itimerspec64(const struct itimerspec64 *it,
			struct __kernel_itimerspec __user *uit)
{
	int ret;

	ret = put_timespec64(&it->it_interval, &uit->it_interval);
	if (ret)
		return ret;

	ret = put_timespec64(&it->it_value, &uit->it_value);

	return ret;
}
#endif

#if 0
int get_old_itimerspec32(struct itimerspec64 *its,
			const struct old_itimerspec32 __user *uits)
{

	if (__get_old_timespec32(&its->it_interval, &uits->it_interval) ||
	    __get_old_timespec32(&its->it_value, &uits->it_value))
		return -EFAULT;
	return 0;
}
#endif

#if 0
int put_old_itimerspec32(const struct itimerspec64 *its,
			struct old_itimerspec32 __user *uits)
{
	if (__put_old_timespec32(&its->it_interval, &uits->it_interval) ||
	    __put_old_timespec32(&its->it_value, &uits->it_value))
		return -EFAULT;
	return 0;
}
#endif
