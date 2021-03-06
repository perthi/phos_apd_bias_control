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
//#include "RcuRegisterMap.h"
#include "AltroRegisterMap.h"
#include "BCRegisterMap.h"
#include "PhosDataTypes.h"
#include "RcuRegisterMap.h"

using namespace BCRegisterMap;

ScriptCompiler::ScriptCompiler() : PhosDcsBase()
{


}




void 
ScriptCompiler::MakeTriggerConfigScript(const char *filename,  TriggerMode_t triggerMode, const AltroConfig_t altroconfig) 
{
  FILE *fp;
  fp=fopen(filename, "w");
  // int tmp;

  //  int IS_pos = 0;  //intruction pointer relative too the basd address of the instruction memeory
  /*
   fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM     ,MakeMS20Instruction(REGTYPE_ALTRO, false, AltroRegisterMap::TRCFG));
   fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 1 ,MakeLS20Instruction(false, (altroconfig.GetNSamples()).GetIntValue()));
   fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 2, MakeMS20Instruction(REGTYPE_ALTRO, false, AltroRegisterMap::DPCFG));
   fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 3, MakeLS20Instruction(false, (altroconfig.GetReadoutMode().GetIntValue())));
   fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 4, MakeMS20Instruction(REGTYPE_ALTRO, false, AltroRegisterMap::DPCFG2));
   fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 5, MakeLS20Instruction(false, (altroconfig.GetNPreSamples()).GetIntValue()));
   fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 6, RcuRegisterMap::END);
   fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 7, RcuRegisterMap::ENDMEM);

   fprintf(fp, "w 0x%x  0x0\n",  RcuRegisterMap::EXEC);
   fprintf(fp, "wait 200 us\n");
  */

  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::ALTROIF, (altroconfig.GetNSamples()).GetIntValue() | 1<<10 | 3<<14); 

  //  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::RDOMOD, (altroconfig.GetReadoutMode()).GetIntValue());
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::RDOMOD, 0x1 << 3);

  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::ALTROCFG1, (altroconfig.GetReadoutMode()).GetIntValue());
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::ALTROCFG2, (altroconfig.GetNPreSamples()).GetIntValue());  

  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM     ,MakeMS20Instruction(REGTYPE_ALTRO, false, AltroRegisterMap::TRCFG));
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 1 ,MakeLS20Instruction(false, (altroconfig.GetNSamples()).GetIntValue()));
  
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 2, MakeMS20Instruction(REGTYPE_ALTRO, false, AltroRegisterMap::DPCFG));
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 3, MakeLS20Instruction(false, (altroconfig.GetReadoutMode().GetIntValue())));
  
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 4, MakeMS20Instruction(REGTYPE_ALTRO, false, AltroRegisterMap::DPCFG2));
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 5, MakeLS20Instruction(false, (altroconfig.GetNPreSamples()).GetIntValue()));
  
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 6, RcuRegisterMap::END);
  fprintf(fp, "w 0x%x  0x%x\n", RcuRegisterMap::Instruction_MEM + 7, RcuRegisterMap::ENDMEM);
  
  fprintf(fp, "w 0x%x  0x0\n",  RcuRegisterMap::EXEC);
  fprintf(fp, "wait 200 us\n");
  
  //CRAP PTH  (invalid trigger mode, internalluy generated L2)
  int tmp = 0x10000 | (4*( (altroconfig.GetNSamples()).GetIntValue() + (altroconfig.GetNPreSamples()).GetIntValue()) +300);
  // fprintf(fp, "w 0x%x 0x%x\n", RcuRegisterMap::TRCFG, tmp);
  //CRAP PTH
  //  int tmp = 0x10000 | 0x3fff;
  
  
  fclose(fp);
}



void
ScriptCompiler::MakeActivateFeeScript(const char *scriptFilename, const unsigned long afl, const unsigned long branch, 
				      const unsigned long cardIndex, const int onOff) 
{
  FILE *fp;
  fp=fopen(scriptFilename, "w");
  unsigned long  activeFeeList = afl;
  char commandString[100];
  long command;
  unsigned long int  tmpFeeList;
  int shift;
  int cardNumber = cardIndex +1;

  if(branch == 0)
    {
      shift = 0;
    } 
  else if(branch == 1)
    {
      shift = 16;
    } 
  else
    {   
     printf("\nERROR in activating card, branch has invalid value:%d\n", branch);
    }
  tmpFeeList = 1<<(cardNumber+shift);

  if(onOff == TURN_OFF)
   {
     if(activeFeeList & tmpFeeList)
       {
	 activeFeeList = (activeFeeList ^ tmpFeeList);  
       }
    
     printf("\nScriptCompiler::acticefrontendlist = 0x%x\n", activeFeeList);
   }

  else if(onOff == TURN_ON)
    {
      activeFeeList = (activeFeeList | tmpFeeList);
    }


  fprintf(fp,"w 0x%x 0x%x\n",  RcuRegisterMap::AFL, activeFeeList);
  fprintf(fp, "wait 10000 us\n");  
  fprintf(fp, "w 0x%x 0x%x\n", RcuRegisterMap::RDOL, activeFeeList);
  fprintf(fp, "wait 10000 us\n");
  fprintf(fp, "w 0x%x 0x0\n",  RcuRegisterMap::FEC_RESET);
  fprintf(fp, "wait 100000 us\n");  
  fprintf(fp, "w 0x%x 0x0\n",  RcuRegisterMap::FEC_RESET);
  fprintf(fp, "wait 100000 us\n");
  fprintf(fp, "w 0x%x 0xffff\n",RcuRegisterMap::Result_MEM);
  //  sprintf(commandString, "w 0x%x 0x%x\n", RcuRegisterMap::Instruction_MEM ,MakeMS20Instruction(REGTYPE_BC, true, 1, branch, cardNumber, BCVERSION ) );
  sprintf(commandString, "w 0x%x 0x%x\n", RcuRegisterMap::Instruction_MEM ,MakeMS20Instruction(REGTYPE_BC, true, BCVERSION, branch, cardNumber) ); //OD
  fprintf(fp, "%s", commandString);
  fprintf(fp, "w 0x%x 0x%x\n", RcuRegisterMap::Instruction_MEM+1, RcuRegisterMap::END);
  fprintf(fp, "w 0x%x 0x%x\n", RcuRegisterMap::Instruction_MEM+2, RcuRegisterMap::ENDMEM);
  fprintf(fp, "w 0x%x 0x0\n", RcuRegisterMap::EXEC);
  fprintf(fp, "wait 100000 us\n");
  fprintf(fp, "r 0x%x 1\n",  RcuRegisterMap::Result_MEM);
  

  fclose(fp);
}


void
ScriptCompiler::MakeActivateTruScript(const char *scriptFilename, const unsigned long activeFeeList)
{
  FILE *fp;
  fp=fopen(scriptFilename, "w");
  char commandString[100];
  long command;
  unsigned long int  tmpFeeList;
  unsigned long  mask = 0x1 | (1<<16);
  unsigned long  newFeeList=0;
  newFeeList = activeFeeList | mask; 
 
  fprintf(fp,"w 0x%x 0x%x\n",RcuRegisterMap::AFL, newFeeList);
  fprintf(fp, "wait 200000 us\n");  
  fprintf(fp, "w 0x%x 0x%x\n",RcuRegisterMap::RDOL , newFeeList);
  fprintf(fp, "wait 200000 us\n");
  fprintf(fp, "w 0x%x 0x0\n",  RcuRegisterMap::FEC_RESET);
  fprintf(fp, "wait 100000 us\n");  
  fprintf(fp, "w 0x%x 0x0\n",  RcuRegisterMap::FEC_RESET);

  fclose(fp);

}



void
ScriptCompiler::MakeDeActivateTruScript(const char *scriptFilename, const unsigned long activeFeeList)
{
  FILE *fp;
  fp=fopen(scriptFilename, "w");

  char commandString[100];
  long command;
  unsigned long int  newFeeList;
  unsigned long  mask = 0x7ffe7ffe;
  newFeeList = activeFeeList & mask;

  
  fprintf(fp,"w 0x%x 0x%x\n",RcuRegisterMap::AFL, newFeeList);
  fprintf(fp, "wait 200000 us\n");  
  fprintf(fp, "w 0x%x 0x%x\n",RcuRegisterMap::RDOL , newFeeList);
  fprintf(fp, "wait 200000 us\n");
  fprintf(fp, "w 0x%x 0x0\n",  RcuRegisterMap::FEC_RESET);
  fprintf(fp, "wait 100000 us\n");  
  fprintf(fp, "w 0x%x 0x0\n",  RcuRegisterMap::FEC_RESET);

  
}


void
ScriptCompiler::MakeWriteReadRegisterScript(const int regType, const char *fileName, const unsigned long *reg, 
					    const unsigned long *value,  const bool *verify, const int N, 
					    const unsigned long branch,  const unsigned long card, 
					    const unsigned long chip, const unsigned long channel, const bool writeZeroes) const
{
  cout << "ScriptCompiler::MakeFeeReadRcuRegisterScrip, N = " << N  <<endl;

  char resultBuffer[MAX_RESULT_BUFFER_SIZE];
  
  for(int i=0; i< resultBuffer[MAX_RESULT_BUFFER_SIZE]; i++ )
  {
    resultBuffer[i] = 0;
  }				    


  if(CheckFile(fileName, "w") == 0)
    {
      if( (regType == REGTYPE_BC) || (regType ==  REGTYPE_ALTRO) || (regType == REGTYPE_TRU))
	{
	  MakeWriteReadFeeRegisterScript(regType, fileName , reg, value, verify, N, branch, card, writeZeroes);
	}
      //    else if( (regType == REGTYPE_RCU) || (regType == REGTYPE_RCU_ACL) || (regType == REGTYPE_TOR) || (regType == REGTYPE_BUSY))
      else if( (regType == REGTYPE_RCU)  || (regType == REGTYPE_TOR) || (regType == REGTYPE_BUSY))
	{
	  MakeWriteReadRcuRegisterScript(regType, fileName, reg, value, verify, N, writeZeroes);
	}
      else if(  (regType == REGTYPE_RCU_ACL))
	{
	  MakeWriteReadRcuRegisterScript(regType, fileName, reg, value, verify, N, false);
	}

    }
}


void
ScriptCompiler::MakeReadRegisterScript(const int regType, const char *fileName, 
				       const unsigned long *reg, const int N, 
				       const unsigned long branch, const unsigned long  card, 
				       const unsigned long chip, const unsigned long channel) const
{
  if(CheckFile(fileName, "w") == 0)
    {
      if( (regType == REGTYPE_BC) || (regType ==  REGTYPE_ALTRO) || (regType == REGTYPE_TRU))
	{
	  MakeReadFeeRegisterScript(regType, fileName , reg, N, branch, card);
	}
      else if( (regType == REGTYPE_RCU) || (regType == REGTYPE_RCU_ACL) || (regType == REGTYPE_TOR) || (regType == REGTYPE_BUSY))
	{
	  MakeReadRcuRegisterScript(regType, fileName, reg[0] , N);
	}
    }
}



/** changed for RCU_fw2*/
void 
ScriptCompiler::MakeWriteReadFeeRegisterScript(const int registerType, const char *fileName, 
					       const unsigned long *reg, const unsigned long *value, const bool *verify, 
					       const int N, const unsigned long branch, const unsigned long card, 
					       const unsigned long chip, const unsigned long channel, const bool writeZeroes) const
{

  if(  CheckFile(fileName, "w") == 0 )
    {
      FILE *fp = fopen(fileName, "w");
      int currentAddress = 0;
      //    fprintf(fp,"w 0x6000 %d 0xdead\n",N);
      fprintf(fp,"w 0x%x %d 0xdead\n", RcuRegisterMap::Result_MEM , N);  

      if( (registerType == REGTYPE_BC) || (registerType ==  REGTYPE_ALTRO) || (registerType == REGTYPE_TRU) )
	{


	  currentAddress =  RcuRegisterMap::Instruction_MEM;
	  cout << "WRITE commands" << endl;

	  for(int i=0; i<N; i++)
	    {
	      if(i !=0) putc('\n',fp);
	      fprintf(fp,"w 0x%x 0x%x\n", currentAddress, MakeMS20Instruction(registerType, false, reg[i],  branch, card));
	      printf("w 0x%x 0x%x\n",     currentAddress, MakeMS20Instruction(registerType, false, reg[i],  branch, card));

	      currentAddress++;
	      fprintf(fp,"w 0x%x 0x%x",   currentAddress, MakeLS20Instruction(false, value[i])); 
	      printf("w 0x%x 0x%x\n",     currentAddress, MakeLS20Instruction(false, value[i])); 

	      currentAddress++;
	    }
	
  
	  cout << "READ commands" << endl;
	  putc('\n',fp);
	  
	  for(int i=0; i<N; i++)
	    {
	      if(verify[i] == true)
		{
		  fprintf(fp,"w 0x%x 0x%x\n", currentAddress,   MakeMS20Instruction( registerType, true, reg[i],  branch, card));
		  printf("w 0x%x 0x%x\n",     currentAddress,   MakeMS20Instruction( registerType, true, reg[i], branch, card));

		  currentAddress++; 
		}
	    }
	}
    

      fprintf(fp, "w 0x%x 0x%x\n", currentAddress,   RcuRegisterMap::END);
      currentAddress++; 
      fprintf(fp, "w 0x%x 0x%x\n",  currentAddress, RcuRegisterMap::ENDMEM);
      fprintf(fp, "w 0x%x 0x0",   RcuRegisterMap::EXEC );
      fprintf(fp,"\nwait 100 us");
      fprintf(fp,"\nr 0x%x %d\n", RcuRegisterMap::Result_MEM, N); 
      fclose(fp);
    }
}


/** changed for RCU_fw2*/
void 
ScriptCompiler::MakeWriteReadRcuRegisterScript(const int regType, const char *fileName, const unsigned long *reg, 
					       const unsigned long *value, const bool *verify, const int N, const bool writeZeroes) const
{
  //  cout << "ScriptCompiler::MakeWriteReadRcuRegisterScrip, N = " << N  <<endl;

  if(  CheckFile(fileName, "w") == 0 )
    {
      //     FILE *fp_test = fopen("debug","w");
      

      FILE *fp = fopen(fileName, "w");
      cout << fileName << endl;
      fprintf(fp,"w 0x%x  %d 0x0\n",RcuRegisterMap::Active_Channel_List, N);

      for(int i=0; i< N; i++)
	{
	  if( (value[i] == 0)   &&   (writeZeroes == true) )
	    {
	      fprintf(fp, "w 0x%x 0x%x\n", reg[i], value[i]);
	    }
	  else if(value[i] != 0)
	    {
	      fprintf(fp, "w 0x%x 0x%x\n", reg[i], value[i]);
	    }
	}
      
      fprintf(fp, "r 0x%x %d\n",RcuRegisterMap::Active_Channel_List, N);
      fclose(fp);
    }
}
 

/** changed for RCU_fw2*/
void 
ScriptCompiler::MakeReadFeeRegisterScript(const int registerType, const char *fileName, const unsigned long *reg, 
					  const int N, const unsigned long branch, const unsigned long card,
					  const unsigned long chip, const unsigned long channel) const
{
  printf("ScriptCompiler::MakeReadFeeRegisterScrip, filename = %s\n",fileName  );

 if(  CheckFile(fileName, "w") == 0 )
    {
      FILE *fp = fopen(fileName, "w");
      int currentAddress = 0;
      fprintf(fp,"w 0x%x %d 0xdead\n", RcuRegisterMap::Result_MEM  ,N);
      currentAddress =  RcuRegisterMap::Instruction_MEM;
     
      for(int i=0; i<N; i++)
	{
	  fprintf(fp,"w 0x%x 0x%x\n", currentAddress,  MakeMS20Instruction(registerType, true, reg[i], 
									   branch, card));
	  currentAddress++; 
	}
         
       
      fprintf(fp, "w 0x%x 0x%x\n", currentAddress, RcuRegisterMap::END);
      currentAddress++; 
      fprintf(fp, "w 0x%x 0x%x\n", currentAddress, RcuRegisterMap::ENDMEM);
      
      fprintf(fp, "w 0x%x 0x0",   RcuRegisterMap::EXEC ); 
  
      //      fprintf(fp,"\nw 0x%x 0x0", RcuRegisterMap::EXEC);
   
      fprintf(fp,"\nwait 100 us");
      fprintf(fp,"\nr 0x%x %d\n", RcuRegisterMap::Result_MEM, N); 
      fclose(fp);
    }
}


void
ScriptCompiler::MakeReadRcuRegisterScript(const int registerType, const char *fileName, 
					  const unsigned long baseAddress,  const int N) const 
{
  if(  CheckFile(fileName, "w") == 0 )
    {
      FILE *fp = fopen(fileName, "w");
      fprintf(fp, "r 0x%x %d\n", baseAddress, N);
      fclose(fp);
    }
}


/** changed for RCU_fw2*/
unsigned long 
ScriptCompiler::MakeMS20Instruction(const int registerType,   const bool read,  
				    const unsigned long reg,  const unsigned long branch,  
				    const unsigned long card,  const unsigned long chip, 
				    const unsigned long channel)
{
  
  cout << "ScriptCompiler::MakeMS20Instruction: reg ="  << reg << endl;
  cout << "ScriptCompiler::MakeMS20Instruction: branch ="  << branch << endl;
  cout << "ScriptCompiler::MakeMS20Instruction: card ="  << card << endl;
  cout << "ScriptCompiler::MakeMS20Instruction: chip ="  << chip << endl;
  cout << "ScriptCompiler::MakeMS20Instruction: channel ="  << channel << endl;

  int tmp_bc_altro = 0;
  // int tmp_rw = 0;
  int tmpCommand = 0;

  int tmp_broadcast = 0;

  if(registerType == REGTYPE_BC || (registerType == REGTYPE_TRU) )
    {
      tmp_bc_altro = 1; 

    }
  else if (registerType ==  REGTYPE_ALTRO)
    {
      tmp_bc_altro = 0; 
 
      //CRAP PTH: if we are not reading then we are writing, and for the
      //moment the altro is always configured in broadcast mode
 
      
      if(read == false)
	{
	  
	  tmp_broadcast = 1;
	}
      
    }
 
  if(read == true)
    {
      tmpCommand = FEC_RD; 
    } 
 
  else if (read == false)
    {
      tmpCommand = FEC_WR;
    }
 
  // int tmpCommand = 0;
  
  //    return  (tmpCommand << CMD_TYPE)|( tmp_bc_altro << BC_AL )|  (tmp_broadcast << BCAST) |  (branch<<BRANCH) | (card<<FAD) | (reg <<REGAD) | 17;
  
  return  (tmpCommand << CMD_TYPE)|( tmp_bc_altro << BC_AL )|  (tmp_broadcast << BCAST) |  (branch<<BRANCH) | (card<<FAD) | (channel <<REGAD) | reg;

  //  return  (tmpCommand << CMD_TYPE)|( tmp_bc_altro << BC_AL )|  (tmp_broadcast << BCAST) |  (branch<<BRANCH) | (card<<FAD)  | 11;

  //  return  (tmpCommand << CMD_TYPE)|( tmp_bc_altro << BC_AL )|  (tmp_broadcast << BCAST) |(branch<<BRANCH)|(card<<FAD)|(reg << 6);

 
}


unsigned long 
ScriptCompiler::MakeLS20Instruction(const bool read, const unsigned long registerValue) 
{
  int tmpCommand = 0; 
  //int tmp_rw = 0; 
 
  if(read == true)
    {
     tmpCommand  = FEC_RD ; 
    }
  else if (read == false)
    {
      tmpCommand =  FEC_WR;
    }

  return (tmpCommand << CMD_TYPE) | (registerValue << REGVAL);

}



 



/*
unsigned long 
ScriptCompiler::MakeMS20Instruction(const int registerType,   const bool read, const unsigned long pos,  
				    const unsigned long reg,  const unsigned long branch,  
				    const unsigned long card,  const unsigned long chip, const unsigned long channel)
{
  int tmp_bc_altro = 0;
  int tmp_rw = 0;
  int tmp_broadcast = 0;

  if(registerType == REGTYPE_BC || (registerType == REGTYPE_TRU) )
    {
      tmp_bc_altro = 1; 
    }
  else if (registerType ==  REGTYPE_ALTRO)
    {
      tmp_bc_altro = 0; 
 
      //CRAP PTH: if we are not reading then we are writing, and for the
      //moment the altro is always configured in broadcast mode
 
      
   if(read == false)
     {
       //     tmp_broadcast = 1;
       tmp_broadcast = 1;
     }
   
    }
 
  if(read == true)
    {
      tmp_rw = 0; 
    } 
 
  else if (read == false)
    {
      tmp_rw = 1;
    }
  
  return  (1<<CMD_TYPE)|( tmp_bc_altro << BC_AL )|  (tmp_broadcast << BCAST)  |  ( tmp_rw << RW)|(pos <<POS)|(branch<<BRANCH)|(card<<FAD)|(reg <<REGAD);
 

}
*/

/*
unsigned long 
ScriptCompiler::MakeLS20Instruction(const bool read, const unsigned long pos, const unsigned long registerValue) 
{
  
  int tmp_rw = 0; 
 
  if(read == true)
    {
      tmp_rw = 0; 
    }
  else if (read == false)
    {
      tmp_rw = 1;
    }

  return (1 << CMD_TYPE)|(tmp_rw << RW)|(pos << POS)| (registerValue << REGVAL);

}
*/


 
