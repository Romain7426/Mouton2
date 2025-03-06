#include "global.h"
#include "structures.h"



void structures_debut(void) {
#ifdef WIN
  structures_test();
#endif
}


void structures_fin(void) {
}


void structures_test(void) {
#ifdef MACOSX
  //printf("\n%lu\n", (sizeof (uint8_t)));
  //printf("\n%lu\n", (sizeof (uint64_t)));

  assert((sizeof (uint8_t))  == (1UL));
  assert((sizeof (uint16_t)) == (2UL));
  assert((sizeof (uint32_t)) == (4UL));
  assert((sizeof (uint64_t)) == (8UL));

  assert((sizeof (int8_t))  == (1UL));
  assert((sizeof (int16_t)) == (2UL));
  assert((sizeof (int32_t)) == (4UL));
  assert((sizeof (int64_t)) == (8UL));

#else
#ifndef CROSS_COMPILE
  //printf("\n%u\n", (sizeof (uint8_t)));
  //printf("\n%u\n", (sizeof (uint64_t)));
  //printf("\n%u\n", (sizeof (int64_t)));

#if 0
  uint8_t r;
#if (sizeof(r)) == (1U)
  //#if (sizeof(uint8_t)) == (1U)
#error WAAAAH
#endif

  { 
    if ((sizeof (uint8_t)) == (2U)) { 
      printf("DTC"); 
    } 
    else { 
      prog_assert("(sizeof (uint8_t)) == (1U)", "structures.c", 25); 
    } 
  };
#endif

  assert((sizeof (uint8_t))  == (1U));
  assert((sizeof (uint16_t)) == (2U));
  assert((sizeof (uint32_t)) == (4U));
  assert((sizeof (uint64_t)) == (8U));

  assert((sizeof (int8_t))  == (1U));
  assert((sizeof (int16_t)) == (2U));
  assert((sizeof (int32_t)) == (4U));
  assert((sizeof (int64_t)) == (8U));
#endif
#endif
}



