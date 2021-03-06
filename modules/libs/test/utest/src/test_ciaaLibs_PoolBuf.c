/* Copyright 2016, Mariano Cerdeiro
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief This file implements the test of the string library
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Libs CIAA Libraries
 ** @{ */
/** \addtogroup ModuleTests Module Tests
 ** @{ */

/*==================[inclusions]=============================================*/
#include "unity.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaLibs_PoolBuf.h"
#include "mock_ciaaLibs_Maths.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
CIAALIBS_POOLDECLARE(pool, uint32_t, 60);

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

void test_ciaaLibs_poolBufInit(void) {
   int32_t val;
   val = ciaaLibs_poolBufInit((ciaaLibs_poolBufType*)NULL, (void*)pool_buf, pool_status, 60, sizeof(uint32_t));
   TEST_ASSERT_EQUAL_INT(-1, val);

   val = ciaaLibs_poolBufInit(&pool, (void*)NULL, pool_status, 60, sizeof(uint32_t));
   TEST_ASSERT_EQUAL_INT(-1, val);

   val = ciaaLibs_poolBufInit(&pool, (void*)pool_buf, (uint32_t*)NULL, 60, sizeof(uint32_t));
   TEST_ASSERT_EQUAL_INT(-1, val);

   val = ciaaLibs_poolBufInit(&pool, (void*)pool_buf, pool_status, 60, sizeof(uint32_t));
   TEST_ASSERT_EQUAL_INT(1, val);
}


void test_ciaaLibs_poolBufLockAndFree(void) {
   uint32_t * element;
   size_t ret;

   ciaaLibs_getFirstNotSetBit_ExpectAndReturn(0, 0);
   element = ciaaLibs_poolBufLock(&pool);
   TEST_ASSERT_EQUAL_PTR(&pool_buf[0], element);

   ciaaLibs_getFirstNotSetBit_ExpectAndReturn(1, 1);
   element = ciaaLibs_poolBufLock(&pool);
   TEST_ASSERT_EQUAL_PTR(&pool_buf[1], element);

   ciaaLibs_getFirstNotSetBit_ExpectAndReturn(3, 2);
   element = ciaaLibs_poolBufLock(&pool);
   TEST_ASSERT_EQUAL_PTR(&pool_buf[2], element);

   /* simulate full */
   ciaaLibs_getFirstNotSetBit_ExpectAndReturn(7,-1);
   ciaaLibs_getFirstNotSetBit_ExpectAndReturn(0,60-32);
   element = ciaaLibs_poolBufLock(&pool);
   TEST_ASSERT_EQUAL_PTR(NULL, element);

   ret = ciaaLibs_poolBufFree(&pool, &pool_buf[1]);
   TEST_ASSERT_EQUAL_INT(1, ret);

   ciaaLibs_getFirstNotSetBit_ExpectAndReturn(5, 1);
   element = ciaaLibs_poolBufLock(&pool);
   TEST_ASSERT_EQUAL_PTR(&pool_buf[1], element);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

