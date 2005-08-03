/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* This file is automatically generated.  Do not edit. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <liboil/liboiltest.h>
#include <liboil/liboildebug.h>
#include <liboil/liboilprofile.h>

void
_oil_test_marshal_function (void *func, unsigned long *args, int n_args,
    unsigned int pointer_mask, OilProfile *prof)
{
  switch (pointer_mask) {
    case 0x0034:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *,int,int))func)
        ((void *)args[0],(int)args[1],(void *)args[2],(int)args[3],(int)args[4]);
      oil_profile_stop (prof);
      break;
    case 0x001e:
      oil_profile_start (prof);
      ((void (*)(void *,void *,void *,int))func)
        ((void *)args[0],(void *)args[1],(void *)args[2],(int)args[3]);
      oil_profile_stop (prof);
      break;
    case 0x00d4:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *,int,void *,int,int))func)
        ((void *)args[0],(int)args[1],(void *)args[2],(int)args[3],(void *)args[4],(int)args[5],(int)args[6]);
      oil_profile_stop (prof);
      break;
    case 0x000e:
      oil_profile_start (prof);
      ((void (*)(void *,void *,int))func)
        ((void *)args[0],(void *)args[1],(int)args[2]);
      oil_profile_stop (prof);
      break;
    case 0x00d3:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *,int,int,void *,void *))func)
        ((void *)args[0],(int)args[1],(void *)args[2],(int)args[3],(int)args[4],(void *)args[5],(void *)args[6]);
      oil_profile_stop (prof);
      break;
    case 0x001a:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *,int))func)
        ((void *)args[0],(int)args[1],(void *)args[2],(int)args[3]);
      oil_profile_stop (prof);
      break;
    case 0x003a:
      oil_profile_start (prof);
      ((void (*)(void *,void *,int,void *,int))func)
        ((void *)args[0],(void *)args[1],(int)args[2],(void *)args[3],(int)args[4]);
      oil_profile_stop (prof);
      break;
    case 0x006a:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *,int,void *,int))func)
        ((void *)args[0],(int)args[1],(void *)args[2],(int)args[3],(void *)args[4],(int)args[5]);
      oil_profile_stop (prof);
      break;
    case 0x00ea:
      oil_profile_start (prof);
      ((void (*)(void *,void *,int,void *,int,void *,int))func)
        ((void *)args[0],(void *)args[1],(int)args[2],(void *)args[3],(int)args[4],(void *)args[5],(int)args[6]);
      oil_profile_stop (prof);
      break;
    case 0x0074:
      oil_profile_start (prof);
      ((void (*)(void *,void *,int,void *,int,int))func)
        ((void *)args[0],(void *)args[1],(int)args[2],(void *)args[3],(int)args[4],(int)args[5]);
      oil_profile_stop (prof);
      break;
    case 0x0076:
      oil_profile_start (prof);
      ((void (*)(void *,void *,int,void *,void *,int))func)
        ((void *)args[0],(void *)args[1],(int)args[2],(void *)args[3],(void *)args[4],(int)args[5]);
      oil_profile_stop (prof);
      break;
    case 0x001c:
      oil_profile_start (prof);
      ((void (*)(void *,void *,int,int))func)
        ((void *)args[0],(void *)args[1],(int)args[2],(int)args[3]);
      oil_profile_stop (prof);
      break;
    case 0x0007:
      oil_profile_start (prof);
      ((void (*)(void *,void *))func)
        ((void *)args[0],(void *)args[1]);
      oil_profile_stop (prof);
      break;
    case 0x003e:
      oil_profile_start (prof);
      ((void (*)(void *,void *,void *,void *,int))func)
        ((void *)args[0],(void *)args[1],(void *)args[2],(void *)args[3],(int)args[4]);
      oil_profile_stop (prof);
      break;
    case 0x0078:
      oil_profile_start (prof);
      ((void (*)(void *,void *,void *,int,int,int))func)
        ((void *)args[0],(void *)args[1],(void *)args[2],(int)args[3],(int)args[4],(int)args[5]);
      oil_profile_stop (prof);
      break;
    case 0x0001:
      oil_profile_start (prof);
      ((void (*)(void))func)
        ();
      oil_profile_stop (prof);
      break;
    case 0x0035:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *,int,void *))func)
        ((void *)args[0],(int)args[1],(void *)args[2],(int)args[3],(void *)args[4]);
      oil_profile_stop (prof);
      break;
    case 0x00d5:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *,int,void *,int,void *))func)
        ((void *)args[0],(int)args[1],(void *)args[2],(int)args[3],(void *)args[4],(int)args[5],(void *)args[6]);
      oil_profile_stop (prof);
      break;
    case 0x000d:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *))func)
        ((void *)args[0],(int)args[1],(void *)args[2]);
      oil_profile_stop (prof);
      break;
    case 0x001d:
      oil_profile_start (prof);
      ((void (*)(void *,void *,int,void *))func)
        ((void *)args[0],(void *)args[1],(int)args[2],(void *)args[3]);
      oil_profile_stop (prof);
      break;
    case 0x000f:
      oil_profile_start (prof);
      ((void (*)(void *,void *,void *))func)
        ((void *)args[0],(void *)args[1],(void *)args[2]);
      oil_profile_stop (prof);
      break;
    case 0x003b:
      oil_profile_start (prof);
      ((void (*)(void *,void *,int,void *,void *))func)
        ((void *)args[0],(void *)args[1],(int)args[2],(void *)args[3],(void *)args[4]);
      oil_profile_stop (prof);
      break;
    case 0x0353:
      oil_profile_start (prof);
      ((void (*)(void *,int,void *,int,void *,int,int,void *,void *))func)
        ((void *)args[0],(int)args[1],(void *)args[2],(int)args[3],(void *)args[4],(int)args[5],(int)args[6],(void *)args[7],(void *)args[8]);
      oil_profile_stop (prof);
      break;
    default:
      OIL_ERROR ("unhandled marshal case");
  }
}

