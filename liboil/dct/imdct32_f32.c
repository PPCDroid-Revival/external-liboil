/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/dct/dct.h>
#include <math.h>

OIL_DEFINE_CLASS (imdct32_f32, "float *dest, float *src");

static void imdct32_f32_ref (float *dest, float *src)
{
	double x;
	int i,j;
	double coeff;

	for(i=0;i<32;i++){
		x = 0;
		for(j=0;j<32;j++){
			coeff = cos((M_PI/32)*i*(j+0.5));
			x += coeff * src[j];
		}
		dest[i] = x;
	}
}

OIL_DEFINE_IMPL_REF (imdct32_f32_ref, imdct32_f32_class);

/* from mpglib */
/*
 * Discrete Cosine Tansform (DCT) for subband synthesis
 * optimized for machines with no auto-increment. 
 * The performance is highly compiler dependend. Maybe
 * the dct64.c version for 'normal' processor may be faster
 * even for Intel processors.
 */

static void imdct32_f32_mpglib(float *dest,float *samples)
{
	static float cos64[16];
	static float cos32[16];
	static float cos16[16];
	static float cos8[16];
	static float cos4[16];
	float b1[32];
	float b2[32];
	static int done = 0;

	if(!done){
		int k;

		done = 1;
		for(k=0;k<16;k++){
			cos64[k] = 1.0 / (2.0 * cos(M_PI * (k * 2.0 + 1.0) / 64.0));
			cos32[k] = 1.0 / (2.0 * cos(M_PI * (k * 2.0 + 1.0) / 32.0));
			cos16[k] = 1.0 / (2.0 * cos(M_PI * (k * 2.0 + 1.0) / 16.0));
			cos8[k] = 1.0 / (2.0 * cos(M_PI * (k * 2.0 + 1.0) / 8.0));
			cos4[k] = 1.0 / (2.0 * cos(M_PI * (k * 2.0 + 1.0) / 4.0));
		}
	}

 {
  float *costab = cos64;

  b1[0x00] = samples[0x00] + samples[0x1F];
  b1[0x1F] = (samples[0x00] - samples[0x1F]) * costab[0x0];

  b1[0x01] = samples[0x01] + samples[0x1E];
  b1[0x1E] = (samples[0x01] - samples[0x1E]) * costab[0x1];

  b1[0x02] = samples[0x02] + samples[0x1D];
  b1[0x1D] = (samples[0x02] - samples[0x1D]) * costab[0x2];

  b1[0x03] = samples[0x03] + samples[0x1C];
  b1[0x1C] = (samples[0x03] - samples[0x1C]) * costab[0x3];

  b1[0x04] = samples[0x04] + samples[0x1B];
  b1[0x1B] = (samples[0x04] - samples[0x1B]) * costab[0x4];

  b1[0x05] = samples[0x05] + samples[0x1A];
  b1[0x1A] = (samples[0x05] - samples[0x1A]) * costab[0x5];

  b1[0x06] = samples[0x06] + samples[0x19];
  b1[0x19] = (samples[0x06] - samples[0x19]) * costab[0x6];

  b1[0x07] = samples[0x07] + samples[0x18];
  b1[0x18] = (samples[0x07] - samples[0x18]) * costab[0x7];

  b1[0x08] = samples[0x08] + samples[0x17];
  b1[0x17] = (samples[0x08] - samples[0x17]) * costab[0x8];

  b1[0x09] = samples[0x09] + samples[0x16];
  b1[0x16] = (samples[0x09] - samples[0x16]) * costab[0x9];

  b1[0x0A] = samples[0x0A] + samples[0x15];
  b1[0x15] = (samples[0x0A] - samples[0x15]) * costab[0xA];

  b1[0x0B] = samples[0x0B] + samples[0x14];
  b1[0x14] = (samples[0x0B] - samples[0x14]) * costab[0xB];

  b1[0x0C] = samples[0x0C] + samples[0x13];
  b1[0x13] = (samples[0x0C] - samples[0x13]) * costab[0xC];

  b1[0x0D] = samples[0x0D] + samples[0x12];
  b1[0x12] = (samples[0x0D] - samples[0x12]) * costab[0xD];

  b1[0x0E] = samples[0x0E] + samples[0x11];
  b1[0x11] = (samples[0x0E] - samples[0x11]) * costab[0xE];

  b1[0x0F] = samples[0x0F] + samples[0x10];
  b1[0x10] = (samples[0x0F] - samples[0x10]) * costab[0xF];
 }


 {
  float *costab = cos32;

  b2[0x00] = b1[0x00] + b1[0x0F]; 
  b2[0x0F] = (b1[0x00] - b1[0x0F]) * costab[0];
  b2[0x01] = b1[0x01] + b1[0x0E]; 
  b2[0x0E] = (b1[0x01] - b1[0x0E]) * costab[1];
  b2[0x02] = b1[0x02] + b1[0x0D]; 
  b2[0x0D] = (b1[0x02] - b1[0x0D]) * costab[2];
  b2[0x03] = b1[0x03] + b1[0x0C]; 
  b2[0x0C] = (b1[0x03] - b1[0x0C]) * costab[3];
  b2[0x04] = b1[0x04] + b1[0x0B]; 
  b2[0x0B] = (b1[0x04] - b1[0x0B]) * costab[4];
  b2[0x05] = b1[0x05] + b1[0x0A]; 
  b2[0x0A] = (b1[0x05] - b1[0x0A]) * costab[5];
  b2[0x06] = b1[0x06] + b1[0x09]; 
  b2[0x09] = (b1[0x06] - b1[0x09]) * costab[6];
  b2[0x07] = b1[0x07] + b1[0x08]; 
  b2[0x08] = (b1[0x07] - b1[0x08]) * costab[7];

  b2[0x10] = b1[0x10] + b1[0x1F];
  b2[0x1F] = (b1[0x1F] - b1[0x10]) * costab[0];
  b2[0x11] = b1[0x11] + b1[0x1E];
  b2[0x1E] = (b1[0x1E] - b1[0x11]) * costab[1];
  b2[0x12] = b1[0x12] + b1[0x1D];
  b2[0x1D] = (b1[0x1D] - b1[0x12]) * costab[2];
  b2[0x13] = b1[0x13] + b1[0x1C];
  b2[0x1C] = (b1[0x1C] - b1[0x13]) * costab[3];
  b2[0x14] = b1[0x14] + b1[0x1B];
  b2[0x1B] = (b1[0x1B] - b1[0x14]) * costab[4];
  b2[0x15] = b1[0x15] + b1[0x1A];
  b2[0x1A] = (b1[0x1A] - b1[0x15]) * costab[5];
  b2[0x16] = b1[0x16] + b1[0x19];
  b2[0x19] = (b1[0x19] - b1[0x16]) * costab[6];
  b2[0x17] = b1[0x17] + b1[0x18];
  b2[0x18] = (b1[0x18] - b1[0x17]) * costab[7];
 }

 {
  float *costab = cos16;

  b1[0x00] = b2[0x00] + b2[0x07];
  b1[0x07] = (b2[0x00] - b2[0x07]) * costab[0];
  b1[0x01] = b2[0x01] + b2[0x06];
  b1[0x06] = (b2[0x01] - b2[0x06]) * costab[1];
  b1[0x02] = b2[0x02] + b2[0x05];
  b1[0x05] = (b2[0x02] - b2[0x05]) * costab[2];
  b1[0x03] = b2[0x03] + b2[0x04];
  b1[0x04] = (b2[0x03] - b2[0x04]) * costab[3];

  b1[0x08] = b2[0x08] + b2[0x0F];
  b1[0x0F] = (b2[0x0F] - b2[0x08]) * costab[0];
  b1[0x09] = b2[0x09] + b2[0x0E];
  b1[0x0E] = (b2[0x0E] - b2[0x09]) * costab[1];
  b1[0x0A] = b2[0x0A] + b2[0x0D];
  b1[0x0D] = (b2[0x0D] - b2[0x0A]) * costab[2];
  b1[0x0B] = b2[0x0B] + b2[0x0C];
  b1[0x0C] = (b2[0x0C] - b2[0x0B]) * costab[3];

  b1[0x10] = b2[0x10] + b2[0x17];
  b1[0x17] = (b2[0x10] - b2[0x17]) * costab[0];
  b1[0x11] = b2[0x11] + b2[0x16];
  b1[0x16] = (b2[0x11] - b2[0x16]) * costab[1];
  b1[0x12] = b2[0x12] + b2[0x15];
  b1[0x15] = (b2[0x12] - b2[0x15]) * costab[2];
  b1[0x13] = b2[0x13] + b2[0x14];
  b1[0x14] = (b2[0x13] - b2[0x14]) * costab[3];

  b1[0x18] = b2[0x18] + b2[0x1F];
  b1[0x1F] = (b2[0x1F] - b2[0x18]) * costab[0];
  b1[0x19] = b2[0x19] + b2[0x1E];
  b1[0x1E] = (b2[0x1E] - b2[0x19]) * costab[1];
  b1[0x1A] = b2[0x1A] + b2[0x1D];
  b1[0x1D] = (b2[0x1D] - b2[0x1A]) * costab[2];
  b1[0x1B] = b2[0x1B] + b2[0x1C];
  b1[0x1C] = (b2[0x1C] - b2[0x1B]) * costab[3];
 }

 {
  float cos0 = cos8[0];
  float cos1 = cos8[1];

  b2[0x00] = b1[0x00] + b1[0x03];
  b2[0x03] = (b1[0x00] - b1[0x03]) * cos0;
  b2[0x01] = b1[0x01] + b1[0x02];
  b2[0x02] = (b1[0x01] - b1[0x02]) * cos1;

  b2[0x04] = b1[0x04] + b1[0x07];
  b2[0x07] = (b1[0x07] - b1[0x04]) * cos0;
  b2[0x05] = b1[0x05] + b1[0x06];
  b2[0x06] = (b1[0x06] - b1[0x05]) * cos1;

  b2[0x08] = b1[0x08] + b1[0x0B];
  b2[0x0B] = (b1[0x08] - b1[0x0B]) * cos0;
  b2[0x09] = b1[0x09] + b1[0x0A];
  b2[0x0A] = (b1[0x09] - b1[0x0A]) * cos1;
  
  b2[0x0C] = b1[0x0C] + b1[0x0F];
  b2[0x0F] = (b1[0x0F] - b1[0x0C]) * cos0;
  b2[0x0D] = b1[0x0D] + b1[0x0E];
  b2[0x0E] = (b1[0x0E] - b1[0x0D]) * cos1;

  b2[0x10] = b1[0x10] + b1[0x13];
  b2[0x13] = (b1[0x10] - b1[0x13]) * cos0;
  b2[0x11] = b1[0x11] + b1[0x12];
  b2[0x12] = (b1[0x11] - b1[0x12]) * cos1;

  b2[0x14] = b1[0x14] + b1[0x17];
  b2[0x17] = (b1[0x17] - b1[0x14]) * cos0;
  b2[0x15] = b1[0x15] + b1[0x16];
  b2[0x16] = (b1[0x16] - b1[0x15]) * cos1;

  b2[0x18] = b1[0x18] + b1[0x1B];
  b2[0x1B] = (b1[0x18] - b1[0x1B]) * cos0;
  b2[0x19] = b1[0x19] + b1[0x1A];
  b2[0x1A] = (b1[0x19] - b1[0x1A]) * cos1;

  b2[0x1C] = b1[0x1C] + b1[0x1F];
  b2[0x1F] = (b1[0x1F] - b1[0x1C]) * cos0;
  b2[0x1D] = b1[0x1D] + b1[0x1E];
  b2[0x1E] = (b1[0x1E] - b1[0x1D]) * cos1;
 }

 {
  float cos0 = cos4[0];

  b1[0x00] = b2[0x00] + b2[0x01];
  b1[0x01] = (b2[0x00] - b2[0x01]) * cos0;
  b1[0x02] = b2[0x02] + b2[0x03];
  b1[0x03] = (b2[0x03] - b2[0x02]) * cos0;
  b1[0x02] += b1[0x03];

  b1[0x04] = b2[0x04] + b2[0x05];
  b1[0x05] = (b2[0x04] - b2[0x05]) * cos0;
  b1[0x06] = b2[0x06] + b2[0x07];
  b1[0x07] = (b2[0x07] - b2[0x06]) * cos0;
  b1[0x06] += b1[0x07];
  b1[0x04] += b1[0x06];
  b1[0x06] += b1[0x05];
  b1[0x05] += b1[0x07];

  b1[0x08] = b2[0x08] + b2[0x09];
  b1[0x09] = (b2[0x08] - b2[0x09]) * cos0;
  b1[0x0A] = b2[0x0A] + b2[0x0B];
  b1[0x0B] = (b2[0x0B] - b2[0x0A]) * cos0;
  b1[0x0A] += b1[0x0B];

  b1[0x0C] = b2[0x0C] + b2[0x0D];
  b1[0x0D] = (b2[0x0C] - b2[0x0D]) * cos0;
  b1[0x0E] = b2[0x0E] + b2[0x0F];
  b1[0x0F] = (b2[0x0F] - b2[0x0E]) * cos0;
  b1[0x0E] += b1[0x0F];
  b1[0x0C] += b1[0x0E];
  b1[0x0E] += b1[0x0D];
  b1[0x0D] += b1[0x0F];

  b1[0x10] = b2[0x10] + b2[0x11];
  b1[0x11] = (b2[0x10] - b2[0x11]) * cos0;
  b1[0x12] = b2[0x12] + b2[0x13];
  b1[0x13] = (b2[0x13] - b2[0x12]) * cos0;
  b1[0x12] += b1[0x13];

  b1[0x14] = b2[0x14] + b2[0x15];
  b1[0x15] = (b2[0x14] - b2[0x15]) * cos0;
  b1[0x16] = b2[0x16] + b2[0x17];
  b1[0x17] = (b2[0x17] - b2[0x16]) * cos0;
  b1[0x16] += b1[0x17];
  b1[0x14] += b1[0x16];
  b1[0x16] += b1[0x15];
  b1[0x15] += b1[0x17];

  b1[0x18] = b2[0x18] + b2[0x19];
  b1[0x19] = (b2[0x18] - b2[0x19]) * cos0;
  b1[0x1A] = b2[0x1A] + b2[0x1B];
  b1[0x1B] = (b2[0x1B] - b2[0x1A]) * cos0;
  b1[0x1A] += b1[0x1B];

  b1[0x1C] = b2[0x1C] + b2[0x1D];
  b1[0x1D] = (b2[0x1C] - b2[0x1D]) * cos0;
  b1[0x1E] = b2[0x1E] + b2[0x1F];
  b1[0x1F] = (b2[0x1F] - b2[0x1E]) * cos0;
  b1[0x1E] += b1[0x1F];
  b1[0x1C] += b1[0x1E];
  b1[0x1E] += b1[0x1D];
  b1[0x1D] += b1[0x1F];
 }

 dest[ 0] = b1[0x00];
 dest[ 4] = b1[0x04];
 dest[ 8] = b1[0x02];
 dest[12] = b1[0x06];
// dest[0x10* 0] = b1[0x01];  /* I think this is wrong */
 dest[16] = b1[0x01];
 dest[20] = b1[0x05];
 dest[24] = b1[0x03];
 dest[28] = b1[0x07];

 b1[0x08] += b1[0x0C];
 dest[2] = b1[0x08];
 b1[0x0C] += b1[0x0a];
 dest[6] = b1[0x0C];
 b1[0x0A] += b1[0x0E];
 dest[10] = b1[0x0A];
 b1[0x0E] += b1[0x09];
 dest[14] = b1[0x0E];
 b1[0x09] += b1[0x0D];
 dest[18] = b1[0x09];
 b1[0x0D] += b1[0x0B];
 dest[22] = b1[0x0D];
 b1[0x0B] += b1[0x0F];
 dest[26] = b1[0x0B];
 dest[30] = b1[0x0F];

 b1[0x18] += b1[0x1C];
 dest[1] = b1[0x10] + b1[0x18];
 dest[3] = b1[0x18] + b1[0x14];
 b1[0x1C] += b1[0x1a];
 dest[5] = b1[0x14] + b1[0x1C];
 dest[7] = b1[0x1C] + b1[0x12];
 b1[0x1A] += b1[0x1E];
 dest[9] = b1[0x12] + b1[0x1A];
 dest[11] = b1[0x1A] + b1[0x16];
 b1[0x1E] += b1[0x19];
 dest[13] = b1[0x16] + b1[0x1E];
 dest[15] = b1[0x1E] + b1[0x11];
 b1[0x19] += b1[0x1D];
 dest[17] = b1[0x11] + b1[0x19];
 dest[19] = b1[0x19] + b1[0x15];
 b1[0x1D] += b1[0x1B];
 dest[21] = b1[0x15] + b1[0x1D];
 dest[23] = b1[0x1D] + b1[0x13];
 b1[0x1B] += b1[0x1F];
 dest[25] = b1[0x13] + b1[0x1B];
 dest[27] = b1[0x1B] + b1[0x17];
 dest[29] = b1[0x17] + b1[0x1F];
 dest[31] = b1[0x1F];
}

OIL_DEFINE_IMPL (imdct32_f32_mpglib, imdct32_f32_class);



#ifdef TEST_imdct32_f32
int TEST_imdct32_f32(void)
{
	int i;
	int pass;
	int failures = 0;
	f32 *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;
	double sad;
	double sad_max = 0;
	double sad_sum = 0;

	src = sl_malloc_f32(32);
	dest_ref = sl_malloc_f32(32);
	dest_test = sl_malloc_f32(32);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(imdct32_f32_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<32;i++)src[i]=sl_rand_f32_0_1();

		imdct32_f32_ref(dest_ref,src);
		sl_profile_start(t);
		imdct32_f32_FUNC(dest_test,src);
		sl_profile_stop(t);

		sad = 0;
		for(i=0;i<32;i++){
			sad += fabs(dest_test[i] - dest_ref[i]);
		}
		if(sad>sad_max)sad_max = sad;
		sad_sum += sad;
#if 0
		if(sad>0){
			printf("sad = %g\n",sad);
		}
#endif
#if 0
			if(dest_test[i] != dest_ref[i]){
				printf("%d %g %g\n",i,dest_ref[i], dest_test[i]);
			}
#endif
	}

	printf("sad ave = %g\n",sad_sum/N_PASS);
	printf("sad max = %g\n",sad_max);

	sl_free(src);
	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif

