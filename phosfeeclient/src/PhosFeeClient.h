#ifndef PHOSFEECLIENT_H
#define PHOSFEECLIENT_H

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


#include <FeeSampleClient.hpp>
#include "PhosDcsBase.h"

class ScriptCompiler;

using namespace dcs;
using namespace dcs::fee;
using std::vector;


class PhosFeeClient :public FeeSampleClient, public PhosDcsBase
{
 public:
  PhosFeeClient();
  virtual ~PhosFeeClient();
  void SetScripFileName(const char *filename);

  const int WriteReadRegisters(const int regtype, const char *feeServerName, const unsigned long *regs, const unsigned long *values, const bool *verify, 
			 const int N, const int branch = 999, const int card = 999);
  int ReadRegisters(const int regtype, const char *feeServerName, const unsigned long *regs, unsigned long *rbValues,  const int N, const int branch = 999, const int card = 999);

  /**
   * Executes a script given on a given feeserver.
   * @param scriptFilename name of the script that should be executed.
   * @param feeServerName name of the feeserver where the script should
   * be executed.
   * @param resultBuffer pointer to a character buffer to store the command line
   * output from the invocation of rcu-sh at the feeserver.
   * @param N the number of characters to copy into the resultBuffer.
   * if the script does not involve any read operation then N can be set to zero. 
   **/

  void ExecuteScript(const char *scriptFilename, const char *feeServerName, char *resultBuffer, const int N);

  //  void Execute(const char *feeServerName, const long dataBuffer, char *resultBuffer, const int N);
  void ExecuteBinary(const char* feeServerName, const unsigned long* binData, unsigned long* resultBuffer, const int N);
  void ExecuteACLBinary(const char *feeServerName, const unsigned long* binData, unsigned long *resultBuffer, const int N);

  const unsigned int CheckFeeState(const char *feeServerName,  const int branch, const int cardNumber, char *message, unsigned long int *pcmv = 0);

 private:
  PhosFeeClient(const PhosFeeClient & );
  PhosFeeClient & operator = (const PhosFeeClient &)
    {
      return *this;
    };

  //  char fFormatString[10000];

  char formatString[10000];

  char fScriptFilename[1024];

  /** must be changed for RCU_fw2 */
  void ScanValues(unsigned long *values, const char *resultBuffer, const int bufferSize, const unsigned long baseAddress, const int N);


  int  VerifyValues(const unsigned long *values1, const unsigned long *values2, const bool  *verify, const int N) const;

  /** must be changed for RCU_fw2 */
  void MakeFormatString(const int N, char *formatString, const int baseAddress);


  ScriptCompiler *scriptCompilerPtr;
  BinaryCompiler *binaryCompilerPtr;
};


#endif

