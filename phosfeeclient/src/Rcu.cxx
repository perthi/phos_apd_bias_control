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
#include "FeeCard.h"
#include "Rcu.h"
#include "ScriptCompiler.h"
#include "unistd.h"
#include "BCRegisterMap.h"

using namespace dcs::fee;
using namespace BCRegisterMap;


Rcu::Rcu() 
{
  // never to be called
}


Rcu::Rcu(PhosFeeClient *feeClientPtr, const char *feeServerName, const int mId, 
	 const int rId, const int z, const int x) : PhosDcsBase(), 
						    fFeeClientPtr(feeClientPtr), 
						    fModuleId(mId),	 
						    fRcuId(rId),
						    fZCoord(z), 
						    fXCoord(x), 
						    fActiveFeeList(0), 
						    fActiveFeeRdoList(0)
{
  SetFeeServer(feeServerName); //set the feeServerName same as rcu name
  InitFeeCards();
}


Rcu::~Rcu()
{

}



void 
Rcu::ApplyPattern(const Pattern_t pattern, char *message) const
{
  sprintf(message, "Rcu::ApplyPattern, applying pattern for Module %d  RCU %d, please wait..\n", fModuleId, fRcuId);
  cout <<" Rcu::ApplyPattern" << endl;
  pattern.PrintInfo("Rcu::ApplyPattern");
}


int
Rcu::GetRcuId() const
{
  return fRcuId;
}


void
Rcu::ExecuteScript(const char *fileName) const
{
  cout << "Rcu::ExecuteScript fileName = "  << fileName  << endl;

  char resultBuffer[100];
  fFeeClientPtr->ExecuteScript(fileName, fFeeServerName, resultBuffer, 100);
}


void 
Rcu::SetPcmVersion(const unsigned long pcmversion, const int branch, const int cardIndex) const 
{
  cout << endl;
  cout << "Rcu::SetPcmVersion, "<< fFeeServerName  <<"   branch = "  <<  branch << ",  card = " <<  cardIndex  <<endl;
  fFeeCardPtr[branch*14 + cardIndex]->SetPcmversion(pcmversion);
}


unsigned int
Rcu::ActivateFee(const int branch, const int cardIndex)
{
  //  unsigned long tmpAddr = 0x8000;


  unsigned long tmpAddr = RcuRegisterMap::AFL;

  fFeeClientPtr->SetScripFileName("s_readactivefeelist.txt");
  fFeeClientPtr->ReadRegisters(REGTYPE_RCU, fFeeServerName, &tmpAddr, &fActiveFeeList,1); 
  unsigned long  pcmversion = 0;
  //                                 BCVERSION
  unsigned long  pcmversionReg = BCVERSION;
  char resultBuffer[100];

  ScriptCompiler::MakeActivateFeeScript("s_activateFee.txt", fActiveFeeList, branch, cardIndex, TURN_ON);
  fFeeClientPtr->ExecuteScript("s_activateFee.txt", fFeeServerName, resultBuffer, 70);

  //  fFeeClientPtr->SetFilename("s_readactivefeelist2.txt");
  fFeeClientPtr->SetScripFileName("s_readpcmversion.txt");
  fFeeClientPtr->ReadRegisters(REGTYPE_BC, fFeeServerName, &pcmversionReg, &pcmversion, 1, branch, cardIndex +1);

  fFeeClientPtr->SetScripFileName("s_readactivefeelist2.txt");
  fFeeClientPtr->ReadRegisters(REGTYPE_RCU, fFeeServerName, &tmpAddr, &fActiveFeeList,1); 


  if(pcmversion == 0xdead)
    {
      return FEE_STATE_ERROR;
    }
  else if(pcmversion == 0)
    {
     return DCS_NOT_MASTER;
    }
  else if(pcmversion == PCMVERSION)
    {
      //  SetPcmVersion(pcmversion, branch, cardIndex +1);
      SetPcmVersion(pcmversion, branch, cardIndex );
      return FEE_STATE_ON;
    }
  else if(pcmversion == OLD_PCMVERSION)
    {
      //      SetPcmVersion(pcmversion, branch, cardIndex +1); 
      SetPcmVersion(pcmversion, branch, cardIndex); 
      return FEE_STATE_WARNING;
    }
  else
    {
      return UNKNOWN_PCMVERSION;
    }
}


unsigned int
Rcu::DeActivateFee(const int branch, const int cardIndex)
{
  //  unsigned long tmpAddr = 0x8000;
  unsigned long tmpAddr = RcuRegisterMap::AFL;


  fFeeClientPtr->ReadRegisters(REGTYPE_RCU, fFeeServerName, &tmpAddr, &fActiveFeeList,1); 
  char resultBuffer[100];
  ScriptCompiler::MakeActivateFeeScript("s_activateFee.txt", fActiveFeeList, branch, cardIndex, TURN_OFF);
  fFeeClientPtr->ExecuteScript("s_activateFee.txt", fFeeServerName, resultBuffer, 70);
  fFeeClientPtr->ReadRegisters(REGTYPE_RCU, fFeeServerName, &tmpAddr, &fActiveFeeList,1);  

  return FEE_STATE_OFF;
}


void  
Rcu::UpdateAFL()
{
  unsigned long tmpAddr = 0x8000;
  fFeeClientPtr->ReadRegisters(REGTYPE_RCU, fFeeServerName, &tmpAddr, &fActiveFeeList,1);  
}


int  
Rcu::ToggleFeeOnOff(const int branch, const int cardNumber)
{
  printf("\nRcu::ToggleFeeOnOff; feestates = \n");

  for(int i=0; i < CARDS_PER_RCU ; i++)
    {
      printf("%d  ",*fFeeState[i] );
      if(i == 13)
	{
	  printf("\n");
	} 
    }
  int state=0;
  char resBuffer[100];
  char message[100];
  int cardIndex = cardNumber - 1;

  if(cardNumber == 14)
    {
      ToggleFeeOnOff(branch, 1);
    }
  else if(cardNumber ==13)
    {
      ToggleFeeOnOff(branch, 2);  
    }

  else
    {
      if((*fFeeState[branch*14 + cardIndex] == FEE_STATE_ON) || (*fFeeState[branch*14 + cardIndex]== FEE_STATE_WARNING) || (*fFeeState[branch*14 + cardIndex] == FEE_STATE_ERROR))
	{
	  *fFeeState[branch*14+cardIndex] = DeActivateFee(branch, cardIndex);
	}
      else
	{
	  *fFeeState[branch*14+cardIndex] = ActivateFee(branch, cardIndex); 
	}

      if(cardIndex == 0)
	{
	  if((*fFeeState[branch*14 + 13] == FEE_STATE_ON) || (*fFeeState[branch*14 + 13] == FEE_STATE_WARNING)|| (*fFeeState[branch*14 + 13] == FEE_STATE_ERROR))
	    {
	      *fFeeState[branch*14 + 13] = DeActivateFee(branch, 13);
	    }
	  
	  else
	    {
	      *fFeeState[branch*14 + 13] = ActivateFee(branch, 13);
	    }
	}
 
      if(cardIndex == 1)
	{
	  if((*fFeeState[branch*14+12] == FEE_STATE_ON) || (*fFeeState[branch*14+12] == FEE_STATE_WARNING) || (*fFeeState[branch*14+12] == FEE_STATE_ERROR))
	    {
	      *fFeeState[branch*14+12] = DeActivateFee(branch, 12);
	    }
	  else
	    {
	      *fFeeState[branch*14+12] = ActivateFee(branch, 12);
	    }
	}	
    }
  return state;
}



void
//Rcu::TurnOnAllFee(int **status)
Rcu::TurnOnAllFee()
{
  int tmpState;
  //  unsigned long tmpAddr = 0x8000;
 
  unsigned long tmpAddr = RcuRegisterMap::AFL;

  fFeeClientPtr->ReadRegisters(REGTYPE_RCU, fFeeServerName, &tmpAddr, &fActiveFeeList,1);

  unsigned long int one = 1;
  unsigned long int two = 2;
 
  for(unsigned long int i = 0; i<14; i++)
    {
      if(i == 0 || i == 1)
	{
	  if((fActiveFeeList & (one<<(i+(unsigned long int)12)+one)) == 0)
	    {
	      printf("\nactivating %s, branch %d, card %d ", fFeeServerName, BRANCH_A, 12+i+1);
	      *fFeeState[12+i] =  ActivateFee(BRANCH_A, 12+i);
	      printf(", state = %d ", *fFeeState[12+i]);
	    } 
	}

      if((fActiveFeeList & (one<<(i + one))) == 0)
	{     
	  printf("\nactivating %s, branch %d, card %d ", fFeeServerName, BRANCH_A, i+1);
	  *fFeeState[i] =  ActivateFee(BRANCH_A, i);
	  printf(", state = %d ", *fFeeState[i]);
	}
    }
  
  for(unsigned long int i = 14; i<28; i++)
    {
      if(i == 14 || i == 15)
	{
	  if((fActiveFeeList & (one << (i+(unsigned long int)12+two+one))) == 0)
	    {
	      printf("\nactivating %s, branch %d, card %d ", fFeeServerName, BRANCH_B, i - CARDS_PER_BRANCH +1);
	      *fFeeState[12 + i] = ActivateFee(BRANCH_B, i+12-CARDS_PER_BRANCH);
	      printf(", state = %d ", *fFeeState[12+i]); 
	    }
	} 
      
      if((fActiveFeeList & (one << (i+two+one))) == 0)
	{
	  printf("\nactivating %s, branch %d, card %d ", fFeeServerName, BRANCH_B, i+1); 
	  *fFeeState[i] = ActivateFee(BRANCH_B, i - CARDS_PER_BRANCH);
	  printf(", state = %d ", *fFeeState[i]); 
	}
    }
}



void
//Rcu::TurnOffAllFee(int **status) 
Rcu::TurnOffAllFee() 
{
  unsigned long int one = 1;
  unsigned long int two = 2;
  //  unsigned long tmpAddr = 0x8000;
  
  unsigned long tmpAddr = RcuRegisterMap::AFL ;
 
  fFeeClientPtr->ReadRegisters(REGTYPE_RCU, fFeeServerName, &tmpAddr, &fActiveFeeList,1);
 
  for(unsigned long int i = 0; i<14; i++)
    {
      if(i == 0 || i == 1)
	{  
	  
	  if((fActiveFeeList & (one<<(i+(unsigned long int)12)+one)) != 0)
	    {
	      *fFeeState[12+i] = DeActivateFee(BRANCH_A, i+12);
	    }
	}

      if((fActiveFeeList & (one<<(i + one))) != 0)
	{ 
	  *fFeeState[i] = DeActivateFee(BRANCH_A, i);
	}
    }
  
  for(unsigned long i=14; i< CARDS_PER_RCU; i++)
    {
      if(i == 14 || i == 15)
	{  
	  if((fActiveFeeList & (one << (i+(unsigned long int)12+two+one))) != 0)
	    {	  
	      *fFeeState[i+12] =  DeActivateFee(BRANCH_B, 12+i);
	    }
	} 
      if((fActiveFeeList & (one << (i+two+one))) != 0)
	{
	  *fFeeState[i] = DeActivateFee(BRANCH_B, i - CARDS_PER_BRANCH);
	} 
    }

  for(int i =0; i<CARDS_PER_RCU; i++)
    {
      *fFeeState[i] = FEE_STATE_OFF;
    }
}


void
Rcu::SetReadoutRegion(const unsigned long int afl, const int acl[RcuRegisterMap::Active_Channel_List_Length])
{
  //  fActiveChList = acl;


  for(int i=0; i< RcuRegisterMap::Active_Channel_List_Length; i++)
    {
      fActiveChList[i] = acl[i];
    }

  fActiveFeeRdoList = afl;
  cout << "setting readout region" << endl;
}


const int
Rcu::ApplyReadoutRegion() const
{
  int iRet = -1;
  unsigned long tmpAfl[ RcuRegisterMap::Active_Channel_List_Length ]; 
  unsigned long tmpRegs[ RcuRegisterMap::Active_Channel_List_Length ];
  bool tmpVerify[ RcuRegisterMap::Active_Channel_List_Length ];
  unsigned long tmpAddress = RcuRegisterMap::Active_Channel_List; 

  int n = 0;
  char messageBuffer[200];

  for(int i=0; i< RcuRegisterMap::Active_Channel_List_Length ; i++)
    {
      tmpAfl[i] = 0;
      tmpRegs[i] = 0;
      tmpVerify[i] = false;
    }

  for(int i=0; i< RcuRegisterMap::Active_Channel_List_Length ; i++)
    {
      
      tmpAfl[i]    = fActiveChList[i] ;
      tmpRegs[i]   = tmpAddress;

      if(fActiveChList[i] !=0)
	{
	  tmpVerify[i] = false;
	}
      else
	{
	  tmpVerify[i] = false;
	}

      tmpAddress ++;
    }

  char tmp[256];
  sprintf(tmp,"s_afl_%s.txt", fFeeServerName);

  fFeeClientPtr->SetScripFileName(tmp);

  iRet = fFeeClientPtr->WriteReadRegisters(REGTYPE_RCU_ACL, fFeeServerName, tmpRegs, tmpAfl, tmpVerify, RcuRegisterMap::Active_Channel_List_Length );   

  if(iRet == REG_OK)
    {
      printf("The activre channel list was set correctly for %s, status = %d", fFeeServerName, iRet);
    }
  else
    {
      printf("WARNING:Active channel list was not set correctly for %s, status = %d", fFeeServerName, iRet);
   }
  cout << "Rcu::ApplyReadoutRegion() finnsihed applying readout reagio" << endl; 
  return iRet;

}


void
Rcu::ArmTrigger(const char *fileName, char *message)
{
  cout << "Rcu::ArmTrigge, filename = " <<  fileName  << endl;
  char resultBuffer[50];
  sprintf(message, "Arming trigger for server: %s  ...... Done ! ", fFeeServerName);
  fFeeClientPtr->ExecuteScript(fileName, fFeeServerName, resultBuffer, 50);
}


void 
Rcu::ApplyTruSettings(const unsigned long regAddr[N_TRU_REGS], const unsigned long regVal[N_TRU_REGS], const bool verify[N_TRU_REGS], const int N, char *message) const 
{
  int iRet = 0;

  for(int i =0; i< TRUS_PER_RCU; i++)
    {
      iRet =  fFeeClientPtr->WriteReadRegisters(REGTYPE_TRU, fFeeServerName, regAddr, regVal, verify, N, BRANCH_A, TRU_SLOT); 
      
      if(iRet == REG_OK)
	{
	  //	  sprintf(message[i], "applying register settings for TRU %d of %s  ..SUCCESS", i, fFeeServerName);
	}
      else if(iRet == REG_DEAD)
	{
	  //	  sprintf(message[i], "applying reg settings for TRU %d of %s ..ERROR 1: No response from TRU" , i, fFeeServerName);	  
	}
      else if(iRet == REG_ZERO)
	{
	  //	  sprintf(message[i], "applying register settings for TRU %d of %s ..ERROR 2: Cannot acsess RCU\n", i, fFeeServerName);	 
	}          
      else if(iRet == REG_CRAZY)
	{
	  //	  sprintf(message[i], "applying register settings for TRU %d of %s ..ERROR 3: Readback values are crazy", i, fFeeServerName);	 
	}
      
      else
	{
	  //	  sprintf(message[i], "applying register settings for TRU %d of %sthree quarks for muster mark ..oops. something is very wrong, consult a psyciatrist", i, fFeeServerName);
	}
    }
}



void
Rcu::EnableTrigger() const
{
  char resultBuffer[50];
  char tmpFileName[1024];  
  //  sprintf(tmpFileName,"%s/s_enabletrigger.txt" ,fPhosDcsScriptDir ); 
  sprintf(tmpFileName,"s_enabletrigger.txt"); 
  fFeeClientPtr->ExecuteScript(tmpFileName, fFeeServerName, resultBuffer, 50);
  //  cout <<   "Enabeling external RCU trigger for : %s  ...... Done ! "  << endl;
  printf("Enabeling external RCU trigger for : %s  ...... Done ! ", fFeeServerName);
}


void
Rcu::EnableTrigger_ttcrx() const
{
  char resultBuffer[50];
  char tmpFileName[1024];  
  // sprintf(tmpFileName,"%s/s_enabletrigger_ttcrx.txt" ,fPhosDcsScriptDir );  
  sprintf(tmpFileName,"s_enabletrigger_ttcrx.txt");  
  fFeeClientPtr->ExecuteScript( tmpFileName, fFeeServerName, resultBuffer, 50);
  printf("Enabeling ttcrx trigger for : %s  ...... Done ! ", fFeeServerName);
  //  cout <<  "Enabeling trigger over TTCRx for: %s  ...... Done ! " << endl;
}


void
Rcu::DisArmTrigger() const
{
  char tmpFileName[1024];
  char resultBuffer[50];
  // sprintf(tmpFileName,"%s/s_disarmtrigger.txt" ,fPhosDcsScriptDir );
 sprintf(tmpFileName,"s_disarmtrigger.txt");
  fFeeClientPtr->ExecuteScript(tmpFileName,  fFeeServerName, resultBuffer, 50);
}


unsigned int 
Rcu::CheckFeeState(const int branch, const int cardNumber, char *message)
{
  unsigned int tmpStatus;
  unsigned long int tmpPcmversion;
  int tmpIndex = branch*CARDS_PER_BRANCH + cardNumber -1;


  if(IsActiveFee(branch, cardNumber) == false)
    {
      sprintf(message,"branch %d card %d : FEE_STATE_OFF", cardNumber,  branch);
      *fFeeState[tmpIndex] = FEE_STATE_OFF;
      return  FEE_STATE_OFF;
    }
  else
    {
  
    }
  

  tmpStatus = fFeeClientPtr->CheckFeeState(fFeeServerName, branch, cardNumber, message, &tmpPcmversion) ;

  if( (tmpPcmversion == PCMVERSION) || (tmpPcmversion ==  OLD_PCMVERSION ))
    {
      SetPcmVersion(tmpPcmversion,  branch , cardNumber -1); 
    }

  *fFeeState[tmpIndex] = tmpStatus;

  return tmpStatus;
}


bool 
Rcu::IsActiveFee(const int branch, const int card) const
{
  cout << endl;
  bool tmp;
  unsigned long int mask = 0x1;
  int shift =  (branch*16 +card);
  mask = mask << shift;
  unsigned long res = (mask & fActiveFeeList);

  if(res == 0)
    {
      tmp = false;
      cout << "Rcu::IsActiveFee, branch "  << branch << "  card " << card <<"  is not active" << endl;
    }
  else
    {
      tmp = true;
      cout << "Rcu::IsActiveFee, branch "  << branch << "  card " << card <<"  IS!!! active" << endl;
    }
  return tmp;
}


unsigned long 
Rcu::GetActiveFeeList()
{
  return fActiveFeeList;
}


int**
Rcu::GetFeeStatus()
{
  return fFeeState;
}


FeeCard * 
Rcu::GetFeeCardPtr(const int index) const
{
  
  return fFeeCardPtr[index];
}


void
Rcu::InitFeeCards()
{
  for(int i = 0; i <= CARDS_PER_BRANCH; i++)
    {
      fFeeCardPtr[i] = new FeeCard(fFeeClientPtr, fFeeServerName, fModuleId, fRcuId, BRANCH_A, i+1);
      fFeeState[i] = fFeeCardPtr[i]->GetState();
    } 

  for(int i = 0; i <= CARDS_PER_BRANCH; i++)
    {
      fFeeCardPtr[i+CARDS_PER_BRANCH] = new FeeCard(fFeeClientPtr, fFeeServerName, fModuleId, fRcuId, BRANCH_B, i+1);
      fFeeState[i+CARDS_PER_BRANCH] = fFeeCardPtr[i+CARDS_PER_BRANCH]->GetState(); 
    } 
}


void
Rcu::LoadApdValues()
{
   for(int i=0; i<CARDS_PER_RCU; i++)
    {
      fFeeCardPtr[i]->LoadApdValues();
    }
}


int 
Rcu::ApplyApdSettings(const int branch, const int card, char *messageBuffer) const 
{
  cout << "Rcu::ApplyApdSetting; fFeeCardPtr["<<  card + branch*CARDS_PER_BRANCH-1 <<"], branch = "<< fFeeCardPtr[card + branch*CARDS_PER_BRANCH-1]->GetBranch();
  cout <<  "card = "<< fFeeCardPtr[card + branch*CARDS_PER_BRANCH-1]->GetCardNumber();
  fFeeCardPtr[card + branch*CARDS_PER_BRANCH-1]->ApplyApdSettings(messageBuffer);
}


char* 
Rcu::GetFeeServerName()
{
  return fFeeServerName; 
}


void
Rcu::SetAllApds(int apdVal)
{
  for(int i=0; i< 28; i++)
    {
      if(fFeeCardPtr[i] != 0)
	{
	  fFeeCardPtr[i]->SetAllApds(apdVal);
	}
      else
	{
	  printf("\nApdInputPtr is ZERO\n");

	}
    }
}


void
Rcu::SetFeeServer(const char *name)
{
  sprintf(fFeeServerName,"%s",name);
  cout << "ServerName was set to: " << fFeeServerName << "." << endl; 
  if( fFeeClientPtr !=0)
    {
      fFeeClientPtr->registerFeeServerName(fFeeServerName); 
       cout << "Registered FeeServername" << endl;
    }
  else
    {
      cout << "Rcu::SetFeeServerName: !!! ERROR, fFeeClientPtr = NULL !!!!!!!!!! " << endl;
    }
}



