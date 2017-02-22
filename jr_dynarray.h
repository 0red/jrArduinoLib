//File: DynamicArrayHelper.h //Created by: S. Schutten, schutten@hotmail.com
 #ifndef DynamicArrayHelper_h
 #define DynamicArrayHelper_h
 //#include <wiring.h>
#include "Arduino.h"

#define jrDYN 															DynamicArrayHelper dynamicarrayhelper      
#define jrDYN_Len(arr,newCount,len,elSize)  dynamicarrayhelper.SetArrayLength   ( (void *&)arr,newCount,len,elSize )
#define jrDYN_Dek(arr,elInx,len,elSize)  		dynamicarrayhelper.RemoveFromArray  ( (void *&)arr,elInx,len,elSize )
#define jrDYN_Set(arr,el,elInx,len,elSize)  dynamicarrayhelper.SetElementInArray( (void *&)arr,&el,elInx,len,elSize )
#define jrDYN_Add(arr,el,len,elSize)  			dynamicarrayhelper.AddToArray				( (void *&)arr,&el,len,elSize )

class DynamicArrayHelper {
 public:
 // Enlarge the array to be able to hold the specified number of elements
 static bool SetArrayLength(void *& array, int newelements , int & elements , byte elementsize);
 //static bool SetArrayLength(void *& array, byte newelements ,                   byte elementsize);


// Store an element in the array
 static bool SetElementInArray(void *& array, void * element, int elementindex , int & elements , byte elementsize);
// static bool SetElementInArray(void *& array, void * element, byte elementindex ,                   byte elementsize);

// Add an element to the array
 static bool AddToArray(void *& array, void * element, int & elements , byte elementsize);
// static bool AddToArray(void *& array, void * element,                   byte elementsize);
 

// Remove an element from the array
 static bool RemoveFromArray(void *& array, int elementindex, int & elements , byte elementsize);
// static bool RemoveFromArray(void *& array, int elementindex,                   byte elementsize) ;
 };
 
  /*
 DynamicArrayHelper dynamicarrayhelper;
 http://playground.arduino.cc/Code/DynamicArrayHelper
 https://github.com/codebendercc

void ProtocolBase::SetPulse(unsigned int *& pulsebuffer, byte & pulsebufferlength, byte pulsepos, unsigned int value)
{  
->dynamicarrayhelper.SetElementInArray( (void *&)pulsebuffer, &value , pulsepos , pulsebufferlength ,  sizeof(unsigned int) );
}
 */

 #endif
 
 