/**************************************************************************
 * This file is property of and copyright by the Experimental Nuclear     *
 * Physics Group, Dep. of Physics                                         *
 * University of Oslo, Norway, 2007                                       *
 *                                                                        *
 * Author: Per Thomas Hille <perthi@fys.uio.no> for the ALICE HLT Project.*
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

#include "DcsInterface.h"
#include "PhosDetector.h"
#include "PhosModule.h"
#include "DatabaseDummy.h"
#include "Rcu.h"
#include "FeeCard.h"
#include "PhosDataTypes.h"

//DcsInterface::DcsInterface() : PhosDcsBase()
DcsInterface::DcsInterface()
{
  fPhosDetectorPtr = new PhosDetector();
  PhosModule* tmpPhosModulePtr = fPhosDetectorPtr->GetModulePtr(MODULE_2);

//   tmpPhosModulePtr->CreateRcu("tpc-fee_0_17_3", MODULE_2, RCU_0, Z_0, X_0);
//   tmpPhosModulePtr->CreateRcu("tpc-fee_0_17_3", MODULE_2, RCU_1, Z_1, X_0);
//   tmpPhosModulePtr->CreateRcu("tpc-fee_0_17_3", MODULE_2, RCU_2, Z_0, X_1);
//   tmpPhosModulePtr->CreateRcu("alphsdcs0280", MODULE_2, RCU_3, Z_1, X_1);
 
 tmpPhosModulePtr->CreateRcu("dcs0327", MODULE_2, RCU_0, Z_0, X_0);
 tmpPhosModulePtr->CreateRcu("dcs0327", MODULE_2, RCU_1, Z_1, X_0);
 tmpPhosModulePtr->CreateRcu("dcs0327", MODULE_2, RCU_2, Z_0, X_1);
 tmpPhosModulePtr->CreateRcu("dcs0327", MODULE_2, RCU_3, Z_1, X_1);

 fPhosDetectorPtr->StartFeeClient();
 fDatabasePtr = new DatabaseDummy();
}



DcsInterface::~DcsInterface()
{

}


void 
DcsInterface::ApplyApdSettings(const int modID, const int rcuId, const int branch, const int card, char *messageBuffer) const 
{
  fPhosDetectorPtr->ApplyApdSettings(modID, rcuId, branch, card, messageBuffer);
}

void 
DcsInterface::ApplyTruSettings(int modID, int RcuID, char *Mesbuf, unsigned long *regAddr, unsigned long *regVal, bool *verify, int nTruRegs)
{
  //  fPhosDetectorPtr->phosModulePtr[modID]->rcuPtr[RcuID]->ApplyTruSettings(regAddr, regVal, verify, nTruRegs,  Mesbuf);
}


int 
DcsInterface::ArmTrigger(const int modID) const
{
  cout << "DcsInterface::ArmTrigger" << endl;
  return fPhosDetectorPtr->ArmTrigger(modID);
}


unsigned int 
DcsInterface::CheckFeeState(const int mod,  const int rcu , const int branch , const int cardId, char *message)
{
  Rcu *tmpRcuPtr =  fPhosDetectorPtr->GetRcuPtr(mod, rcu); 
  return tmpRcuPtr->CheckFeeState(branch, cardId, message);
}


void 
DcsInterface::DisArmTrigger(const int modID, const int RcuID, char *messageBuffer) const
{
  fPhosDetectorPtr->DisArmTrigger(modID, RcuID, messageBuffer);
}


void 
DcsInterface::EnableTrigger(int modID)
{
  fPhosDetectorPtr->phosModulePtr[modID]->EnableTrigger();
}


void 
DcsInterface::EnableTrigger_ttcrx(int modID)
{
  fPhosDetectorPtr->phosModulePtr[modID]->EnableTrigger_ttcrx();
}


unsigned long*
DcsInterface::GetApdValues(const int mod, const int rcu, const int branch, const int cardId)
{
  FeeCard *tmpFeeCard  = GetFeeCard(mod,  rcu, branch, cardId);
  return tmpFeeCard->GetApdValues();
}


void
DcsInterface::GetConfigComment(char *text, const int id)
{
  fDatabasePtr->GetConfigComment(text, id);
}


FeeCard* 
DcsInterface::GetFeeCard(const int mod, const int rcu, const int branch, const int cardId) const
{
  int tmpFeeIndex = branch*CARDS_PER_BRANCH + cardId -1;

  Rcu *tmpRcuPtr =  fPhosDetectorPtr->GetRcuPtr(mod, rcu); 
  FeeCard *tmpFeeCard = tmpRcuPtr->GetFeeCardPtr(tmpFeeIndex);
}


int
DcsInterface::GetLatestConfigId()
{
  return fDatabasePtr->GetLatestConfigId();
}


Rcu* 
DcsInterface::GetRcuPtr(int modID, int rcuID) const
{
  return fPhosDetectorPtr->GetRcuPtr(modID, rcuID);
}


void      
DcsInterface::LoadApdConfig(ConfigInfo_t *info) //Raed apd values from datbase and put them in sandbox directory
{
  fDatabasePtr->LoadApdConfig(info);
}


void      
DcsInterface::LoadApdConfig(ConfigInfo_t *info, int id) //Raed apd values from datbase and put them in sandbox directory
{
  fDatabasePtr->LoadApdConfig(info, id);
}

void 
DcsInterface::LoadApdValues(int modID)  //Load apd values into Feecard objects
{
  fPhosDetectorPtr->phosModulePtr[modID]->LoadApdValues();
}


void            
DcsInterface::LoadReadoutConfiguration(ReadoutConfig_t *rdoconfigPtr) const
{
  fDatabasePtr->LoadReadoutConfiguration(rdoconfigPtr);
  rdoconfigPtr->PrintInfo("DcsInterface::LoadReadoutConfiguration ");
} 


void       
DcsInterface::SaveReadoutConfiguration(const ReadoutConfig_t rdoconfig) const
{
  fDatabasePtr->SaveRadoutConfiguration(rdoconfig);
}

void            
DcsInterface::LoadTruSettings(TRUSettings_t *trusettings) const
{
  fDatabasePtr->LoadTruSettings(trusettings);
}

int 
DcsInterface::SaveApdConfig(char *description)
{
  fDatabasePtr->SaveApdConfig(description); 
}


void            
DcsInterface::SaveTRUSettings(const TRUSettings_t trusettings)
{
  trusettings.PrintInfo("DcsInterface::SaveTRUSettings"); 
  fDatabasePtr->SaveTruSettings(trusettings);
}


void 
DcsInterface::SetAllApds(const int modID, const int apdValue)
{
  fPhosDetectorPtr->phosModulePtr[modID]->SetAllApds(apdValue);
}


void 
DcsInterface::SetAllApds(const int modID, const int rcuId, const int apdValue)
{
  Rcu *tmpRcu =fPhosDetectorPtr->phosModulePtr[modID]->GetRcuPtr(rcuId);
  tmpRcu->SetAllApds(apdValue);
  //  fPhosDetectorPtr->phosModulePtr[modID]->SetAllApds(apdValue);
}


void 
DcsInterface::SetAllApds(const int modID, const int rcuId, const int branch, const int card, const int apdValue)
{
  //  fPhosDetectorPtr->phosModulePtr[modID]->SetAllApds(apdValue);
  FeeCard *tmpFeeCard;
  Rcu *tmpRcu =fPhosDetectorPtr->phosModulePtr[modID]->GetRcuPtr(rcuId);
  tmpFeeCard = tmpRcu->GetFeeCardPtr(branch*CARDS_PER_BRANCH + card-1);
  tmpFeeCard->SetAllApds(apdValue);
}


void 
DcsInterface::SetApdValues(const int mod, const int rcu, const int branch, const int cardId, const unsigned long *values)
{
  cout <<"DcsInterface::SetApdValues; mod  =" << mod <<"  rcu = "<< rcu <<"  branch = "<< branch << "card = " <<  cardId <<endl;  
  FeeCard *tmpFeeCard  = GetFeeCard(mod,  rcu, branch, cardId);
  tmpFeeCard->SetApdValues(values);
}


void
DcsInterface::SetPhosBit(const int modId) const
{
  fPhosDetectorPtr->SetPhosBit(modId);
}


void 
DcsInterface::SetReadoutConfig(const ModNumber_t modID,  const ReadoutConfig_t rdoConfig , char *messageBuf) const
{
  //  rdoConfig.PrintInfo("DcsInterface::SetReadoutConfig"); 
  fPhosDetectorPtr->SetReadoutConfig(modID,  rdoConfig,  messageBuf);
}


unsigned int 
DcsInterface::ToggleOnOffFee(const int mod,  const int rcu , const int branch , const int cardId, const unsigned int currentstate, unsigned int tmpStates[CARDS_PER_RCU], char *tmpMessage)
{
  Rcu *tmpRcuPtr =  fPhosDetectorPtr->GetRcuPtr(mod, rcu); 
  tmpRcuPtr->ToggleFeeOnOff(branch, cardId);
  int **tmp = tmpRcuPtr->GetFeeStatus();

  for(int i=0; i < CARDS_PER_RCU; i++)
    {
      tmpStates[i] = *tmp[i];
    }
}


void 
DcsInterface::TurnOnAllFee(const int modID, const int rcuId) const
{
  Rcu *tmpRcuPtr =  fPhosDetectorPtr->GetRcuPtr(modID, rcuId); 
  tmpRcuPtr->TurnOnAllFee();
}


void 
DcsInterface::TurnOnAllTru(const int modID, char *message) const
{
  //  fPhosDetectorPtr->phosModulePtr[modID]->TurnOnAllTru(message);
}


void 
DcsInterface::TurnOffAllFee(const int modID, const  int rcuId) const
{
  Rcu *tmpRcuPtr =  fPhosDetectorPtr->GetRcuPtr(modID, rcuId); 
  tmpRcuPtr->TurnOffAllFee();

  //  fPhosDetectorPtr->phosModulePtr[modID]->TurnOffAllFee();
}

void 
DcsInterface::TurnOffAllTru(const int modID, char *message) const
{
  //  fPhosDetectorPtr->phosModulePtr[modID]->TurnOffAllTru(message);
}


void
DcsInterface::UpdateAFL(const int mod, const int rcu) const
{
  Rcu *tmpRcuPtr =  fPhosDetectorPtr->GetRcuPtr(mod, rcu); 
  tmpRcuPtr->UpdateAFL();
}



// unsigned int 
// DcsInterface::TurnOnFee(const int mod,  const int rcu , const int branch , 
// 			const int cardId,  unsigned int tmpStates[CARDS_PER_RCU], char *tmpMessage)
//   //DcsInterface::TurnOnFee(FeePos_t feePos,  unsigned int tmpStates[CARDS_PER_RCU], char *tmpMessage)
// {
 
// }


// unsigned int 
// DcsInterface::TurnOffFee(const int mod,  const int rcu , const int branch , 
// 			 const int cardId, unsigned int tmpStates[CARDS_PER_RCU], char *tmpMessage)
//   //DcsInterface::TurnOffFee(FeePos_t feePos, unsigned int tmpStates[CARDS_PER_RCU], char *tmpMessage)
// {
//   //  FeeCard *tmpFeeCard  = GetFeeCard(mod,  rcu, branch, cardId);
  
// }







