ObjSuf		= o
SrcSuf		= cxx
HeadSuf		= h


ROOTLIBS	= `root-config --glibs`

OBJ_DIR 		= $(PWD)/obj/




SRC_INCLUDE             = -I$(FEECLIENT)/include -I$ /home/phos/rcu_fw2/phos_dcs_new/phosfeeclient/src -I$(ROOTSYS)/include
#SRC_INCLUDE             = -I$(FEECLIENT)/include -I$(PHOS_DCS_BASEDIR)/src -I$(ROOTSYS)/include
#GUI_INCLUDE		= -I$(PWD)/src -I$(ROOTSYS)/include $(SRC_INCLUDE)
GUI_INCLUDE		= -I$(ROOTSYS)/include $(SRC_INCLUDE)


#FEELIBS	  		=       -L$(FEECLIENT)/lib -lFeeClient   -L/home/phos/lib -ldim  -lpthread
#PHOS_LIBS		=  -L$(PHOS_DCS_LIBDIR) -lPhosDcs
PHOS_LIBS		=  -L$(PHOS_DCS_BASEDIR)/src/lib/ -lPhosDcs

DICTO		  	= $(OBJ_DIR)PhosDcsDICT.$(ObjSuf)
DICTS			= PhosDcsDICT.$(SrcSuf)
DICTH			= PhosDcsDICT.$(HeadSuf)



#GUI sources
PHOSDCSWIDGETO		= $(OBJ_DIR)PhosDcsWidget.$(ObjSuf)
PHOSDCSWIDGETS		= PhosDcsWidget.$(SrcSuf)
PHOSDCSWIDGETH		= PhosDcsWidget.$(HeadSuf)

ALTROMENUO		= $(OBJ_DIR)AltroMenu.$(ObjSuf)
ALTROMENUS		= AltroMenu.$(SrcSuf)
ALTROMENUH		= AltroMenu.$(HeadSuf)


ELECTRONICSMENUO	= $(OBJ_DIR)ElectronicsMenu.$(ObjSuf)
ELECTRONICSMENUS	= ElectronicsMenu.$(SrcSuf)
ELECTRONICSMENUH	= ElectronicsMenu.$(HeadSuf)

ELECTRONICSMENUAPPLYO	= $(OBJ_DIR)ElectronicsMenuApply.$(ObjSuf)
ELECTRONICSMENUAPPLYS	= ElectronicsMenuApply.$(SrcSuf)
ELECTRONICSMENUAPPLYH	= ElectronicsMenuApply.$(HeadSuf)

ELECTRONICSMENURCUO	= $(OBJ_DIR)ElectronicsMenuRCU.$(ObjSuf)
ELECTRONICSMENURCUS	= ElectronicsMenuRCU.$(SrcSuf)
ELECTRONICSMENURCUH	= ElectronicsMenuRCU.$(HeadSuf)

ELECTRONICSMENUREADOUTO	= $(OBJ_DIR)ElectronicsMenuReadout.$(ObjSuf)
ELECTRONICSMENUREADOUTS	= ElectronicsMenuReadout.$(SrcSuf)
ELECTRONICSMENUREADOUTH	= ElectronicsMenuReadout.$(HeadSuf)

ELECTRONICSMENUGETUSERDATAO	= $(OBJ_DIR)ElectronicsMenuGetUserData.$(ObjSuf)
ELECTRONICSMENUGETUSERDATAS	= ElectronicsMenuGetUserData.$(SrcSuf)
ELECTRONICSMENUGETUSERDATAH	= ElectronicsMenuGetUserData.$(HeadSuf)


SANDBOXMENUO		= $(OBJ_DIR)SandboxMenu.$(ObjSuf)
SANDBOXMENUS		= SandboxMenu.$(SrcSuf)
SANDBOXMENUH		= SandboxMenu.$(HeadSuf)

LOGVIEWERO		= $(OBJ_DIR)LogViewer.$(ObjSuf)
LOGVIEWERS		= LogViewer.$(SrcSuf)
LOGVIEWERH		= LogViewer.$(HeadSuf)

RCUBUTTONO		= $(OBJ_DIR)RcuButton.$(ObjSuf)
RCUBUTTONS		= RcuButton.$(SrcSuf)
RCUBUTTONH		= RcuButton.$(HeadSuf)

RCUMANAGERO		= $(OBJ_DIR)RcuManager.$(ObjSuf)
RCUMANAGERS		= RcuManager.$(SrcSuf)
RCUMANAGERH		= RcuManager.$(HeadSuf)

MAINMENUO		= $(OBJ_DIR)MainMenu.$(ObjSuf)
MAINMENUS		= MainMenu.$(SrcSuf)
MAINMENUH		= MainMenu.$(HeadSuf)

TRUMENUO	        = $(OBJ_DIR)TruMenu.$(ObjSuf)
TRUMENUS	        = TruMenu.$(SrcSuf)
TRUMENUH	        = TruMenu.$(HeadSuf)

FEECARDBUTTONO		= $(OBJ_DIR)FeeCardButton.$(ObjSuf)
FEECARDBUTTONS		= FeeCardButton.$(SrcSuf)
FEECARDBUTTONH		= FeeCardButton.$(HeadSuf)

FEECARDONOFFBUTTONO	= $(OBJ_DIR)FeeCardOnOffButton.$(ObjSuf)
FEECARDONOFFBUTTONS	= FeeCardOnOffButton.$(SrcSuf)
FEECARDONOFFBUTTONH	= FeeCardOnOffButton.$(HeadSuf)

PHOSNUMBERENTRYO	= $(OBJ_DIR)PhosNumberEntry.$(ObjSuf)
PHOSNUMBERENTRYS	= PhosNumberEntry.$(SrcSuf)
PHOSNUMBERENTRYH	= PhosNumberEntry.$(HeadSuf)

PHOSHEXNUMBERENTRYO	= $(OBJ_DIR)PhosHexNumberEntry.$(ObjSuf)
PHOSHEXNUMBERENTRYS	= PhosHexNumberEntry.$(SrcSuf)
PHOSHEXNUMBERENTRYH	= PhosHexNumberEntry.$(HeadSuf)

PHOSMENUBUTTONO 	= $(OBJ_DIR)PhosMenuButton.$(ObjSuf)
PHOSMENUBUTTONS 	= PhosMenuButton.$(SrcSuf)
PHOSMENUBUTTONH 	= PhosMenuButton.$(HeadSuf)

APDINPUTO		= $(OBJ_DIR)ApdInput.$(ObjSuf)
APDINPUTS		= ApdInput.$(SrcSuf)
APDINPUTH		= ApdInput.$(HeadSuf)

MAINGUIO		= $(OBJ_DIR)MainGui.$(ObjSuf)
MAINGUIS		= MainGui.$(SrcSuf)
MAINGUIH		= MainGui.$(HeadSuf)

PHOSGUIO		= apdgui.$(ObjSuf)
PHOSGUIS		= apdgui.$(SrcSuf)


##OBJS	=$(PHOSGUIO)  $(MAINGUIO) $(ALTROMENUO)  $(RCUBUTTONO) $(FEECARDBUTTONO) $(FEECARDONOFFBUTTONO) $(APDINPUTO) $(PHOSMENUBUTTONO) \
##	$(PHOSNUMBERENTRYO) $(PHOSHEXNUMBERENTRYO) $(LOGVIEWERO) $(SANDBOXMENUO) $(ELECTRONICSMENUO) \
##	$(MAINMENUO) $(TRUMENUO) $(PHOSDCSWIDGETO) $(DICTO)

OBJS	=$(MAINGUIO) $(ALTROMENUO)  $(RCUBUTTONO) $(FEECARDBUTTONO) $(FEECARDONOFFBUTTONO) $(APDINPUTO) $(PHOSMENUBUTTONO) \
	$(PHOSNUMBERENTRYO) $(PHOSHEXNUMBERENTRYO) $(LOGVIEWERO) $(SANDBOXMENUO) \
	$(ELECTRONICSMENUO) $(ELECTRONICSMENUAPPLYO) $(ELECTRONICSMENURCUO)  $(ELECTRONICSMENUREADOUTO)  $(ELECTRONICSMENUGETUSERDATAO)  \
	$(MAINMENUO) $(TRUMENUO) $(PHOSDCSWIDGETO) $(DICTO) $(RCUMANAGERO)


$(LIBDIR)/libApdGui.so:$(OBJS)
	g++  -Wl --export-dynamic  -shared   --gc-sections $(ROOTLIBS) $(ROOTGLIBS) $(FEELIBS)  -o  lib/libApdGui.so $(OBJS) -lc


#$(PHOSGUIO):$(PHOSGUIS) $(PHOSGUIH)
#	g++ -ggdb  -fPIC -ffunction-sections    $(GUI_INCLUDE) -c $(PHOSGUIS) -o $(PHOSGUIO)

$(MAINGUIO):$(MAINGUIS) $(MAINGUIH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(MAINGUIS) -o $(MAINGUIO)

$(DICTO):$(DICTS) $(DICTH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(DICTS) -o $(DICTO)

$(ALTROMENUO):$(ALTROMENUS) $(ALTROMENUH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(ALTROMENUS) -o $(ALTROMENUO)

$(PHOSDCSWIDGETO):$(PHOSDCSWIDGETS) $(PHOSDCSWIDGETH)                
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(PHOSDCSWIDGETS) -o $(PHOSDCSWIDGETO)

$(RCUBUTTONO):$(RCUBUTTONS) $(RCUBUTTONH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(RCUBUTTONS) -o $(RCUBUTTONO)

$(RCUMANAGERO):$(RCUMANAGERS) $(RCUMANAGERH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(RCUMANAGERS) -o $(RCUMANAGERO)

$(MAINMENUO):$(MAINMENUS) $(MAINMENUH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(MAINMENUS) -o $(MAINMENUO)



$(ELECTRONICSMENUO):$(ELECTRONICSMENUS) $(ELECTRONICSMENUH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(ELECTRONICSMENUS) -o $(ELECTRONICSMENUO)

$(ELECTRONICSMENUAPPLYO):$(ELECTRONICSMENUAPPLYS) $(ELECTRONICSMENUAPPLYH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(ELECTRONICSMENUAPPLYS) -o $(ELECTRONICSMENUAPPLYO)

$(ELECTRONICSMENURCUO):$(ELECTRONICSMENURCUS) $(ELECTRONICSMENURCUH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(ELECTRONICSMENURCUS) -o $(ELECTRONICSMENURCUO)

$(ELECTRONICSMENUREADOUTO):$(ELECTRONICSMENUREADOUTS) $(ELECTRONICSMENUREADOUTH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(ELECTRONICSMENUREADOUTS) -o $(ELECTRONICSMENUREADOUTO)

$(ELECTRONICSMENUGETUSERDATAO):$(ELECTRONICSMENUGETUSERDATAS) $(ELECTRONICSMENUGETUSERDATAH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(ELECTRONICSMENUGETUSERDATAS) -o $(ELECTRONICSMENUGETUSERDATAO)



$(TRUMENUO):$(TRUMENUS) $(TRUMENUH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(TRUMENUS)  -o $(TRUMENUO)

$(SANDBOXMENUO):$(SANDBOXMENUS) $(SANDBOXMENUH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(SANDBOXMENUS)  -o $(SANDBOXMENUO)

$(LOGVIEWERO):$(LOGVIEWERS) $(LOGVIEWERH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(LOGVIEWERS)  -o $(LOGVIEWERO)

$(FEECARDO):$(FEECARDS) $(FEECARDH)
	g++ -ggdb -fPIC -ffunction-sections  $(SRC_INCLUDE)  -c $(FEECARDS) -o $(FEECARDO)

$(FEECARDBUTTONO):$(FEECARDBUTTONS) $(FEECARDBUTTONH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(FEECARDBUTTONS)  -o $(FEECARDBUTTONO)

$(FEECARDONOFFBUTTONO):$(FEECARDONOFFBUTTONS) $(FEECARDONOFFBUTTONH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(FEECARDONOFFBUTTONS) -o $(FEECARDONOFFBUTTONO)

$(APDINPUTO):$(APDINPUTS) $(APDINPUTH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(APDINPUTS) -o  $(APDINPUTO)

$(PHOSNUMBERENTRYO):$(PHOSNUMBERENTRYS) $(PHOSNUMBERENTRYH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(PHOSNUMBERENTRYS)  -o $(PHOSNUMBERENTRYO)

$(PHOSHEXNUMBERENTRYO):$(PHOSHEXNUMBERENTRYS) $(PHOSHEXNUMBERENTRYH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(PHOSHEXNUMBERENTRYS)  -o $(PHOSHEXNUMBERENTRYO)

$(PHOSMENUBUTTONO):$(PHOSMENUBUTTONS) $(PHOSMENUBUTTONH)
	g++ -ggdb -fPIC -ffunction-sections  $(GUI_INCLUDE)  -c $(PHOSMENUBUTTONS)    -o $(PHOSMENUBUTTONO)

clean:
	@rm -f  *.o $(OBJ_DIR)/*o   *~ *#


