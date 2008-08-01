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

#include "ScriptCompiler.h"
#include "BinaryCompiler.h"
#include "PhosFeeClient.h"
#include <vector>
#include <string>
#include "unistd.h"
#include "BCRegisterMap.h"
#include "RcuRegisterMap.h"

using namespace BCRegisterMap;

using std::cout;
using std::endl;

using std::vector;


PhosFeeClient::PhosFeeClient(): FeeSampleClient(), PhosDcsBase(), scriptCompilerPtr(0)
{
  scriptCompilerPtr = new ScriptCompiler();
  binaryCompilerPtr = new BinaryCompiler();
  sprintf(fScriptFilename, "s_tmp.txt");
  //  sprintf(fScriptFilename, "/home/phos/testbench/s_tmp.txt");

}




PhosFeeClient::~PhosFeeClient()
{
  delete scriptCompilerPtr;
  delete binaryCompilerPtr;
}


void 
PhosFeeClient::SetScripFileName(const char *filename)
{
  cout << "PhosFeeClient::SetScripFileName. filename was set too"  <<  filename << endl;
  sprintf(fScriptFilename, "%s", filename);
}



PhosFeeClient::PhosFeeClient(PhosFeeClient const&):  FeeSampleClient(), PhosDcsBase(),scriptCompilerPtr(0) 
{

}


const int
PhosFeeClient::WriteReadRegisters(const int regType, const char *feeServerName, const unsigned long *regs, 
				  const unsigned long *values, const bool *verify, 
				  const int N, const int branch, const int card) 
{
  int iRet =0;
  unsigned int tmpBufferSize = N*MAX_WORD_SIZE;
  cout << "tmpBufferSize: " << tmpBufferSize << " MAX_WORD_SIZE: " << MAX_WORD_SIZE << " sizeof(unsigned long): " << sizeof(long) << endl;
  char resultBuffer[tmpBufferSize]; 
  unsigned long *tmpScannedValues = new unsigned long[N];

  for(int i=0; i< tmpBufferSize ; i++)
  {
    resultBuffer[i] = 0;
  }

  if(CheckFile(fScriptFilename, "w") == 0)
    {
      if( (regType == REGTYPE_BC) || (regType ==  REGTYPE_ALTRO) || (regType == REGTYPE_TRU))
	{
	  cout << "PhosFeeClient::WriteReadRegister N ="<< N  << endl;
  	  scriptCompilerPtr->MakeWriteReadRegisterScript(regType, fScriptFilename, regs, values, verify, N, branch, card);
	  ExecuteScript(fScriptFilename, feeServerName, resultBuffer, tmpBufferSize); 

	  cout << "PhosFeeClient::WriteReadRegister, printing acnovledge data" << endl;
	  for(int i=0; i <tmpBufferSize; i++)
	    {
	      printf("%c",resultBuffer[i]);
	    }

	  ScanValues(tmpScannedValues, resultBuffer, tmpBufferSize, RcuRegisterMap::Result_MEM, N);
	  iRet = VerifyValues(tmpScannedValues, values, verify, N);
	}
      else if( (regType == REGTYPE_RCU) || (regType == REGTYPE_RCU_ACL) || (regType == REGTYPE_TOR) || (regType == REGTYPE_BUSY))
	{
	  unsigned long tmpMin = MinValue(regs, N);
	  
	  if( regType == REGTYPE_RCU_ACL)
	    {
	      unsigned long* binaryData = new unsigned long[N+3];
	      unsigned int tmpRegType = REGTYPE_RCU_MEM;
	      binaryCompilerPtr->MakeWriteReadRegisterBinary(tmpRegType, binaryData, regs, values, verify, N, branch, card, false);
	      unsigned long tmpResultValues[N];
	      ExecuteBinary(feeServerName, binaryData, tmpResultValues, N);
	      //ExecuteACLBinary(feeServerName, values, tmpResultValues, N);
	      iRet = VerifyValues(tmpResultValues, values, verify, N);
	      delete [] binaryData;
	      
	    }
	  else
	    {
	      scriptCompilerPtr->MakeWriteReadRegisterScript(regType, fScriptFilename, regs, values, verify, N, branch, card, true);
	      ExecuteScript(fScriptFilename, feeServerName, resultBuffer, tmpBufferSize); 
	    
	      ScanValues(tmpScannedValues, resultBuffer, tmpBufferSize, tmpMin, N);
	      iRet = VerifyValues(tmpScannedValues, values, verify, N);
	    }
	}
    }
  return iRet;
}

 

int 
PhosFeeClient::ReadRegisters(const int regType, const char *feeServerName, const unsigned long *regs, unsigned long *rbValues, const int N, const int branch, const int card)
{  
  int iRet =0;
  unsigned int tmpBufferSize = N*MAX_WORD_SIZE;
  char resultBuffer[tmpBufferSize]; 

  unsigned long tmpMin = MinValue(regs, N);

  for(int i=0; i< tmpBufferSize ; i++)
  {
    resultBuffer[i] = 0;
  }
  if(CheckFile(fScriptFilename, "w") == 0)
    {
      scriptCompilerPtr->MakeReadRegisterScript(regType, fScriptFilename, regs, N, branch, card);
      ExecuteScript(fScriptFilename, feeServerName, resultBuffer, tmpBufferSize); 
      if( (regType == REGTYPE_BC) || (regType ==  REGTYPE_ALTRO) || (regType == REGTYPE_TRU) )
	{
	  ScanValues(rbValues, resultBuffer, tmpBufferSize, RcuRegisterMap::Result_MEM, N);
	}
      else if( (regType == REGTYPE_RCU) || (regType == REGTYPE_RCU_ACL) || (regType == REGTYPE_TOR) || (regType == REGTYPE_BUSY)  )
	{
	  ScanValues(rbValues, resultBuffer, tmpBufferSize, tmpMin, N);
	}
   }
  return iRet;
}


int
PhosFeeClient::VerifyValues(const unsigned long *values1, const unsigned long *values2, const bool *verify, const int N) const
{
  int regStatus = REG_OK;;

  for(int i=0; i<N; i++)
    {
      if(verify[i] == true)
	{  
	  printf("PhosFeeClient::VerifyValues: value1[%d] = %d,, value2[%d] = %d\n", i, values1[i], i, values2[i]);
	  if(values1[i] != values2[i])
	    {
	      
	      if(values1[i] == 0xdead)
		{
		  //	  printf("PhosFeeClient::VerifyValues: value1[%d] = %d,, value2[%d] = %d\n", i, values1[i], i, values2[i]);
		  regStatus = REG_DEAD;
		}
	      else if(values1[i] == 0x0)
		{
		  regStatus = REG_ZERO;
		}
	      else
		{
		  regStatus = REG_CRAZY;
		}
	    }
	}
    }
  return regStatus;
}


void
PhosFeeClient::ScanValues(unsigned long *values, const char *resultBuffer, const int bufferSize, const unsigned long baseAddress, const int N)
{
  // cout << "bufferSize =" << bufferSize <<endl;
  // cout << "N = " << N << endl;
  //  char *formatString = new char[bufferSize];
  
  MakeFormatString(N, formatString, baseAddress); 

  int cnt = 0;
  int lines = 0;

  if(N%4 != 0)
    {
      lines = N/4 + 1;
    }
  else
    {
      lines = N/4;
    }

  char tmpFormat[lines][100];
  char tmpBuf[lines][100];
  int fTotCnt = 0;
  int rTotCnt = 0;
  unsigned long tmpVal[lines*4]; 
  //  printf("\nlines = %d\n", lines);

  for(int i=0; i< lines; i++)
    {
      cnt = 0;

      while((formatString[fTotCnt]  != '\n')  &&  (fTotCnt <  bufferSize) )
 	{
	  if(cnt <  100)
	    {
	      tmpFormat[i][cnt] = formatString[fTotCnt];
	      cnt ++;
	      fTotCnt ++;
	    }
	  else
	    {
	      fTotCnt ++;
	    }
	}

      fTotCnt ++;
      tmpFormat[i][cnt] = 0;
      cnt = 0;
 
      while( (resultBuffer[rTotCnt] != '\n')  &&  (rTotCnt <  bufferSize) )
	{
	  if( rTotCnt <  bufferSize)
	    {  
	    
	      if(cnt <  100)
		    {
		      tmpBuf[i][cnt] =resultBuffer[rTotCnt];
		      cnt ++;
		      rTotCnt ++;
		    }
	      else
		{
		  rTotCnt ++;		  
		}
	    }
	}
 
      rTotCnt ++;
      tmpBuf[i][cnt] = 0;
   }

  for(int i = 0; i < lines; i++)
    {
      sscanf(tmpBuf[i], tmpFormat[i], &tmpVal[0 + i*4], &tmpVal[1 + i*4], &tmpVal[2 + i*4], &tmpVal[3 + i*4]);
    }
  
 
  for(int i=0; i<N; i++)
    {
      values[i] = tmpVal[i];
    }

  //  delete[]  formatString;
}


void
PhosFeeClient::MakeFormatString(const int N, char *formatString, const int baseAddr) 
{
  char tmp[1000];
  int cnt = 0;

  int baseAddress = baseAddr;

  sprintf(formatString,"0x%x:",baseAddress);

  for(int i=0; i< N; i++)
    {
      if( (i%4 == 0) && (i != 0))
	{
	  baseAddress = baseAddress +4;
	  sprintf(tmp,"0x%x:", baseAddress);
	  formatString = strcat(formatString, "\n");
	  formatString = strcat(formatString, tmp);
	}
      formatString = strcat(formatString, " %x");
    }
}


void
PhosFeeClient::ExecuteScript(const char *scriptFilename, const char *feeServerName, char *resultBuffer, const int N)
{
  cout << "PhosFeeClient::ExecuteScript,scriptFilenam = " << scriptFilename << endl;

  vector<unsigned int> data;
  data.resize(200000);
  //  data.resize(20000);

  char* charData=(char*)&data[0];  
  
    if(CheckFile(scriptFilename, "r") == 0)
      {
	FILE *fp;
	fp=fopen(scriptFilename, "r");
	int cnt=0;
	char tmp=fgetc(fp);

	while(tmp !=EOF && cnt<(200000-2)*4)
	{
	  charData[cnt+4]=tmp;
	  tmp=getc(fp);
	  cnt++;
	}
      
	fclose(fp);
      
	if(cnt%4)
	  {
	    for(int i=0; i<cnt%4; i++)
	      {
		charData[cnt+4+i]=0;
		cnt++;
	      }
	}
 
	data[0]=FEESRV_RCUSH_SCRIPT;
	data[(cnt/4)+1]= CE_CMD_TAILER;
	std::string serverName    = feeServerName;
	
	size_t size               = (1+(cnt/4)+1)*4;
	cout << "cnt: " << cnt << " size: " << size << endl;
	unsigned short flags = 0;
	short errorCode      = 0;
	short status         = 0;
	
	writeReadData(serverName, size, data, flags, errorCode, status);

	for(int i=0; i< N-1; i++)
	  {
	    resultBuffer[i]=charData[i];
	  }
	
	resultBuffer[N-1] =0;
     }
}

void 
PhosFeeClient::ExecuteBinary(const char* feeServerName, const unsigned long* binData, unsigned long* resultBuffer, const int N)
{
  vector<unsigned int> data;
  data.resize(200000);
  std::string serverName    = feeServerName;
	
  size_t size               = (N+3)*4;
  unsigned short flags = 0;
  short errorCode      = 0;
  short status         = 0;

  writeReadData(serverName, size, data, flags, errorCode, status);


  for(int i=0; i< N-1; i++)
    {
      resultBuffer[i] = binData[i+2];
    }
  resultBuffer[N-1] = 0;
}
void
PhosFeeClient::ExecuteACLBinary(const char *feeServerName, const unsigned long *dataBuffer, unsigned long *resultBuffer, const int N)
{
  cout << "PhosFeeClient::ExecuteBinary" << endl;

  vector<unsigned int> data;
  data.resize(200000);
  //  data.resize(20000);

  /*
  unsigned long* binData=(unsigned long*)&data[0];  


  for(int i = 0; i < N; i++)
    {
      binData[i+1] = dataBuffer[i];
    }

  binData[0] = RcuRegisterMap::RCU_EXEC_INSTRUCTION;
  binData[N+1] = RcuRegisterMap::CE_CMD_TRAILER_B;
  */

  std::string serverName    = feeServerName;
	
  size_t size               = (N+3)*4;
  unsigned short flags = 0;
  short errorCode      = 0;
  short status         = 0;


  unsigned long* binData=(unsigned long*)&data[0];  
  unsigned long baseACLAdd = RcuRegisterMap::Active_Channel_List;

  cout << "0x" << hex << RcuRegisterMap::RCU_WRITE_MEMBLOCK << " - N: " << dec <<  N << endl;

  data[0] = RcuRegisterMap::RCU_WRITE_MEMBLOCK|N;

  data[1] = baseACLAdd;

  data[N+2] = RcuRegisterMap::CE_CMD_TRAILER;

  cout << "Header: 0x" << hex << data[0] << " - TRAILER: 0x" << data[1] << endl;

  for(int i = 0; i < N; i++)
    {
      data[i+2] = dataBuffer[i];
    }
  
  writeReadData(serverName, size, data, flags, errorCode, status);

  /*
  cout << "PhosFeeClient::ExecuteBinary::writeReadData --- N = " << N << endl;
  writeReadData(serverName, size, data, flags, errorCode, status);
  cout << "done" << endl;
  */

  for(int i=0; i< N-1; i++)
    {
      resultBuffer[i] = binData[i+2];
    }
  
  resultBuffer[N] = 0;
}

const unsigned int
PhosFeeClient::CheckFeeState(const char *feeServerName, const int branch, const int cardNumber,  char *message, unsigned long int *pcmv)
{
  unsigned long pcmversion = 0;
  unsigned long address = BCVERSION;

  ReadRegisters(REGTYPE_BC, feeServerName, &address, &pcmversion, 1, branch, cardNumber);
  
//   cout << "PhosFeeClient::CheckFeeState, feeservername ="<< feeServerName  << endl;
//   cout << "PhosFeeClient::CheckFeeState, address = "  <<  address <<endl,
//   cout << "PhosFeeClient::CheckFeeState, card = " << cardNumber << "branch = " << branch <<"   pcmversion = " << pcmversion  << endl;

  
  if(pcmv != 0)
    {
      *pcmv = pcmversion;
    }

  if(pcmversion == 0xffff)
    {
      sprintf(message, "checking.. branch %d,  card %d pcmversion = %d, FEE_STATE_ERROR",  branch, cardNumber, pcmversion); 
      return FEE_STATE_ERROR;
    }
  else if(pcmversion == PCMVERSION)
    {
      sprintf(message, "checking.. branch %d,  card %d pcmversion = %d, FEE_STATE_ON",  branch, cardNumber, pcmversion);   
      return FEE_STATE_ON;
    }
  else if(pcmversion == 0)
    {
      sprintf(message, "checking.. branch %d,  card %d pcmversion = %d, FEE_STATE_ERROR",  branch, cardNumber, pcmversion);    
      return FEE_STATE_ERROR;   
    }
  else if(pcmversion == OLD_PCMVERSION)
    {
      sprintf(message, "checking.. branch %d,  card %d pcmversion = %d, FEE_STATE_WARNING, old pcmversion",  branch, cardNumber, pcmversion);     
     return FEE_STATE_WARNING;
    }
  else
    {
      sprintf(message, "checking.. branch %d,  card %d pcmversion = %d, FEE_STATE_ERROR, unknown",  branch, cardNumber, pcmversion);  
      return FEE_STATE_ERROR;
    }
}

