/**************************************************************************
 * This file is property of and copyright by the Experimental Nuclear     *
 * Physics Group, Dep. of Physics                                         *
 * University of Oslo, Norway, 2006                                       *
 *                                                                        * 
 * Author: Per Thomas Hille perthi@fys.uio.no for the ALICE DCS Project.  *
 * Contributors are mentioned in the code where appropriate.              *
 * Please report bugs to perthi@fys.uio.no                                * 
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#include "BinaryCompiler.h"
#include "RcuRegisterMap.h"

BinaryCompiler::BinaryCompiler() : PhosDcsBase()
{

}

BinaryCompiler::~BinaryCompiler()
{

}

void 
BinaryCompiler::MakeWriteReadRegisterBinary(const unsigned int regType, unsigned long* binData, const unsigned long *reg, 
					    const unsigned long *value,  const bool *verify, const int N, 
					    const  int branch,  const int card, 
					    const bool writeZeroes)
{

  if(regType == REGTYPE_RCU_MEM)
    {
      MakeWriteReadRcuMemoryBlockBinary(binData, reg[0], value, N);
    }
  if(regType == REGTYPE_ALTRO)
    {
    }
  if(regType == REGTYPE_BC)
    {
    }
  if(regType == REGTYPE_TRU)
    {
    }

}


void 
BinaryCompiler::MakeWriteReadRcuMemoryBlockBinary(unsigned long* binData, const unsigned long baseReg, const unsigned long* value, 
						  const int N)
{
  binData[0] = RcuRegisterMap::RCU_WRITE_MEMBLOCK|N;

  binData[1] = baseReg;

  binData[N+2] = RcuRegisterMap::CE_CMD_TRAILER;

  for(int i = 0; i < N; i++) 
    {
      binData[i] = value[i];
    }
}
