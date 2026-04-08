/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "AIExport.h"
#include "AISCommands.h"
#include "ExternalAI/Interface/AISEvents.h"


#include <stdexcept>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>















void initData_store(int skirmishAIId);












// static std::map<int, springai::OOAICallback*> myAICallbacks;

const static int ERROR_SHIFT = 100;

#define CATCH_CPP_AI_EXCEPTION(RETURN_VAR)					\
	catch (int err) {										\
		RETURN_VAR = err;									\
	} catch (const std::exception& e) {						\
		RETURN_VAR = ERROR_SHIFT + 2;						\
	} catch (const std::string& s) {						\
		RETURN_VAR = ERROR_SHIFT + 3;						\
	} catch (const char* s) {								\
		RETURN_VAR = ERROR_SHIFT + 4;						\
	} catch (...) {											\
		RETURN_VAR = ERROR_SHIFT + 5;						\
	}


const SSkirmishAICallback* myCallback = nullptr;

EXPORT(int) init(int skirmishAIId, const struct SSkirmishAICallback* innerCallback) {

	myCallback = innerCallback;

	initData_store(skirmishAIId);

	int ret = 0;

	return ret; // (ret != 0) => error
}

EXPORT(int) release(int skirmishAIId) {


	int ret = 0;

	return ret; // (ret != 0) => error
}




void sendMsg(int skirmishAIId, std::string msg){
	msg = "/say "+msg;
	SSendTextMessageCommand cmd = {msg.c_str(), 1};

	myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_SEND_TEXT_MESSAGE, &cmd);
	return;
}




bool parseMoveCommand(int skirmishAIId, std::string msg);
bool parseUnitGetter(int skirmishAIId, std::string msg);
bool parseGeneralCommand(int skirmishAIId, std::string msg);






struct data_store{
	float metalMap[300];
	int totalMetalPoints = 0;
};

data_store dat;


void initData_store(int skirmishAIId){
	myCallback->Map_getResourceMapSpotsPositions(skirmishAIId,0,dat.metalMap,300);
	std::cout << "testing: " << dat.metalMap[299] << std::endl;

	while(dat.metalMap[dat.totalMetalPoints] != 0){
		dat.totalMetalPoints += 3;
	}


}














EXPORT(int) handleEvent(int skirmishAIId, int topic, const void* data) {

	//			if(myCallback != nullptr){
	//			   std::cout << "myCallback initted" << std::endl;
	//		    }

	switch (topic) {


	case EventTopic::EVENT_INIT:{
		// sendMsg(skirmishAIId, "Initializing!");

		break;
	}

	    case EventTopic::EVENT_UPDATE: {
	    	const SUpdateEvent* event = static_cast<const SUpdateEvent*>(data);
	    	int frame = event->frame;
	    	// std::cout << "frame: " << frame << std::endl; // works -l

	        
	        break;
	    }
		case EventTopic::EVENT_UNIT_CREATED: {
			const SUnitCreatedEvent* event = static_cast<const SUnitCreatedEvent*>(data);
			int unitId = event->unit;
			int builder = event->builder;
			std::cout << "unit "<< unitId << " created by " << builder << std::endl;

			if(builder == -1){
				// unit is created by engine
			}



			// what unit is this



			///


			break;
		}
		case EventTopic::EVENT_UNIT_FINISHED: {
			const SUnitFinishedEvent* event = static_cast<const SUnitFinishedEvent*>(data);
			int unitId = event->unit;
			std::cout << "unit "<< unitId << " finished" << std::endl;


			std::string tempstr = std::string("/say HEyO my id is ")+std::to_string(unitId);
			const char* text = tempstr.c_str();
			SSendTextMessageCommand cmd = {text, 1};
			myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_SEND_TEXT_MESSAGE, &cmd);



			//try to move // this works.

			// float pos[3] = {50.0f,50.0f,50.0f};
			// SMoveUnitCommand cmd2 = {};
			// cmd2.unitId = unitId;
			// cmd2.options = 0;
			// cmd2.toPos_posF3 = pos;

			// myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_UNIT_FIGHT, &cmd2);

			// what is my unit?

			int unitDefId = myCallback->Unit_getDef(skirmishAIId, unitId);
			const char* unitHumanName = myCallback->UnitDef_getName(skirmishAIId, unitDefId);
			std::cout << unitDefId << "the created unit is " << unitHumanName << std::endl;






			break;
		}
	case EventTopic::EVENT_MESSAGE: {
		const SMessageEvent* event = static_cast<const SMessageEvent*>(data);
		int player = event->player;
		const char* msg = event->message;

		std::cout << "player " << player << " said: " << msg << std::endl;

		std::string myMsg = std::string(msg);

		if(myMsg=="you suck"){
			SSendTextMessageCommand cmd = {"/say SHUT THE FUCK UP", 1};
			myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_SEND_TEXT_MESSAGE, &cmd);
		}
		parseMoveCommand(skirmishAIId,myMsg);
		parseUnitGetter(skirmishAIId,myMsg);
		parseGeneralCommand(skirmishAIId,myMsg);


		break;
	}


	    default: //remember to break!!! -l
	    	switch(topic){
	        case 0:{std::cout<< "event: EVENT_NULL for skrimish id: " << skirmishAIId << std::endl; break;}
			case 1:{std::cout<< "event: EVENT_INIT for skrimish id: " << skirmishAIId << std::endl; break;}
			case 2:{std::cout<< "event: EVENT_RELEASE for skrimish id: " << skirmishAIId << std::endl; break;}
			case 3:{std::cout<< "event: EVENT_UPDATE for skrimish id: " << skirmishAIId << std::endl; break;}
			case 4:{std::cout<< "event: EVENT_MESSAGE for skrimish id: " << skirmishAIId << std::endl; break;}
			case 5:{std::cout<< "event: EVENT_UNIT_CREATED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 6:{std::cout<< "event: EVENT_UNIT_FINISHED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 7:{std::cout<< "event: EVENT_UNIT_IDLE for skrimish id: " << skirmishAIId << std::endl; break;}
			case 8:{std::cout<< "event: EVENT_UNIT_MOVE_FAILED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 9:{std::cout<< "event: EVENT_UNIT_DAMAGED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 10:{std::cout<< "event: EVENT_UNIT_DESTROYED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 11:{std::cout<< "event: EVENT_UNIT_GIVEN for skrimish id: " << skirmishAIId << std::endl; break;}
			case 12:{std::cout<< "event: EVENT_UNIT_CAPTURED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 13:{std::cout<< "event: EVENT_ENEMY_ENTER_LOS for skrimish id: " << skirmishAIId << std::endl; break;}
			case 14:{std::cout<< "event: EVENT_ENEMY_LEAVE_LOS for skrimish id: " << skirmishAIId << std::endl; break;}
			case 15:{std::cout<< "event: EVENT_ENEMY_ENTER_RADAR for skrimish id: " << skirmishAIId << std::endl; break;}
			case 16:{std::cout<< "event: EVENT_ENEMY_LEAVE_RADAR for skrimish id: " << skirmishAIId << std::endl; break;}
			case 17:{std::cout<< "event: EVENT_ENEMY_DAMAGED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 18:{std::cout<< "event: EVENT_ENEMY_DESTROYED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 19:{std::cout<< "event: EVENT_WEAPON_FIRED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 20:{std::cout<< "event: EVENT_PLAYER_COMMAND for skrimish id: " << skirmishAIId << std::endl; break;}
			case 21:{std::cout<< "event: EVENT_SEISMIC_PING for skrimish id: " << skirmishAIId << std::endl; break;}
			case 22:{std::cout<< "event: EVENT_COMMAND_FINISHED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 23:{std::cout<< "event: EVENT_LOAD for skrimish id: " << skirmishAIId << std::endl; break;}
			case 24:{std::cout<< "event: EVENT_SAVE for skrimish id: " << skirmishAIId << std::endl; break;}
			case 25:{std::cout<< "event: EVENT_ENEMY_CREATED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 26:{std::cout<< "event: EVENT_ENEMY_FINISHED for skrimish id: " << skirmishAIId << std::endl; break;}
			case 27:{std::cout<< "event: EVENT_LUA_MESSAGE for skrimish id: " << skirmishAIId << std::endl; break;}
			default:{std::cout<< "wtf unknown event occured" << std::endl;}
			}
	}

	

	int ret = 0;

	return ret; // (ret != 0) => error
}


























bool parseMoveCommand(int skirmishAIId, std::string msg) {

    std::stringstream ss(msg);
    std::string cmdstr;
    int unit;
    int option = 0;
    float f[3];

    if (!(ss >> cmdstr) || cmdstr != "move") {
        return false;
    }

    if (!(ss >> unit >> f[0] >> f[1] >> f[2])) {
        return false;
    }

    ss >> option; // um this is bad? i guess it defaults to 0 and works in this case but it wont for other

    std::string extra;
    if (ss >> extra) {
    	//command was longer than expected
    }



	SMoveUnitCommand cmd = {};
	cmd.unitId = unit;
	cmd.options = option;
	cmd.toPos_posF3 = f;

	myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_UNIT_FIGHT, &cmd);

    return true;
}


bool parseUnitGetter(int skirmishAIId, std::string msg) {

    std::stringstream ss(msg);
    std::string cmdstr;
    int unit;
    int amt;

    if (!(ss >> cmdstr) || cmdstr != "ug") {
        return false;
    }
    if (!(ss >> unit >> amt)) {
        return false;
    }

	for(int i = unit; i < amt; i++){
		const char* thisUnitName = myCallback->UnitDef_getName(skirmishAIId, i);
		const char* thisUnitHumanName = myCallback->UnitDef_getHumanName(skirmishAIId, i);
		std::cout << i << ": the created unit is: " << thisUnitName << " human name: " << thisUnitHumanName << " ! " << std::endl;
	}


    return true;
}


bool parseGeneralCommand(int skirmishAIId, std::string msg) {

    std::stringstream ss(msg);
    std::string cmdstr;
    std::string cmdid;

    if (!(ss >> cmdstr) || cmdstr != "cmd") {
        return false;
    }
    if (!(ss >> cmdid)) {
        return false;
    }

    std::cout << "COMMAND STARTED: " << cmdstr << std::endl;

    if(cmdid == "0"){

    	int unitDefId;
    	if(!(ss >> unitDefId)){
    		return false;
    	}

    	int ids[581], count = myCallback->UnitDef_getBuildOptions(skirmishAIId, unitDefId, ids, 581);

    	for (int i = 0; i < count; ++i) std::cout << "Build Option " << i << ": " << ids[i] << std::endl;
    	std::cout << count << std::endl;
    } else if(cmdid == "build"){

    	int unitToBuild;
    	int unitId;
    	int options;

    	if (!(ss >> unitId >> unitToBuild)) { // EXAMPLE CALL: cmd build 6901 383 <32>
        	return false;
    	}
    	if(!(ss >> options)){
    		options = 0; // i know its already zero, but it sets it to zero. so if it ever needs to be anything else, change here
    	}

    	SBuildUnitCommand cmd = {};
		cmd.unitId = unitId;
		cmd.options = options;

		cmd.toBuildUnitDefId = unitToBuild;
		float f2[3];
		myCallback->Unit_getPos(skirmishAIId, unitId, f2);

		std::cout << f2[0] << " " << f2[1] << " " << f2[2] << std::endl; 

		cmd.buildPos_posF3 = f2;
		cmd.facing = UNIT_COMMAND_BUILD_NO_FACING;

		myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_UNIT_BUILD, &cmd);
    } else if(cmdid == "selfd"){

    	int unitId;
    	if (!(ss >> unitId)) { // EXAMPLE CALL: cmd selfd 6901
        	return false;
    	}

    	SSelfDestroyUnitCommand cmd = {};
		cmd.unitId = unitId;

		myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_UNIT_SELF_DESTROY, &cmd);
    } else if(cmdid == "moved"){

    	int unitId;
    	int options;
    	float delta[2];

    	if (!(ss >> unitId >> delta[0] >> delta[1])) { // EXAMPLE CALL: cmd moved 6901 5 15 <32>
        	return false;
    	}
    	if(!(ss >> options)){
    		options = 0; 
    	}

    	SMoveUnitCommand cmd = {};
		cmd.unitId = unitId;
		cmd.options = options;
		cmd.timeOut = 9999999;

		float pos[3];
		myCallback->Unit_getPos(skirmishAIId, unitId, pos);
		pos[0] += delta[0];
		pos[2] += delta[1];
		cmd.toPos_posF3 = pos;

		myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_UNIT_MOVE, &cmd);
    } else if(cmdid == "move"){

    	int unitId;
    	int options;
    	float pos[3];

    	if (!(ss >> unitId >> pos[0] >> pos[1] >> pos[2])) { // EXAMPLE CALL: cmd moved 6901 5 15 <32>
        	return false;
    	}
    	if(!(ss >> options)){
    		options = 0; 
    	}

    	SMoveUnitCommand cmd = {};
		cmd.unitId = unitId;
		cmd.options = options;
		cmd.timeOut = 9999999;
		cmd.toPos_posF3 = pos;

		myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_UNIT_MOVE, &cmd);
    }  else if(cmdid == "nearestMex"){
    	// (CALLING_CONV *Map_getResourceMapSpotsNearest)(int skirmishAIId, int resourceId, float* pos_posF3, float* return_posF3_out)
    	int unitId;
    	int resourceId;

    	if (!(ss >> unitId)) { // EXAMPLE CALL: cmd moved 6901 5 15 <32>
        	return false;
    	}
    	if(!(ss >> resourceId)){
    		resourceId = 0; 
    	}



		float pos[3];
		myCallback->Unit_getPos(skirmishAIId, unitId, pos);
		float posOut[3];
		myCallback->Map_getResourceMapSpotsNearest(skirmishAIId, resourceId, pos, posOut);

		std::stringstream tss;
		tss << "mex pos is: " << posOut[0] << ", " << posOut[1] << ", " << posOut[2] << std::endl;
		sendMsg(skirmishAIId,tss.str());

    }  else if(cmdid == "mexall"){
    	// (CALLING_CONV *Map_getResourceMapSpotsNearest)(int skirmishAIId, int resourceId, float* pos_posF3, float* return_posF3_out)
    	int unitId;

    	if (!(ss >> unitId)) { // EXAMPLE CALL: cmd mexall
        	return false;
    	}

    	for(int i = 0; i < dat.totalMetalPoints; i++){
    		float pos[3];
    		int mpos = i*3;
    		pos[0] = dat.metalMap[mpos+0];
    		pos[1] = dat.metalMap[mpos+1];
    		pos[2] = dat.metalMap[mpos+2];

    		std::stringstream tss;
    		tss << "cmd move " << unitId << " " << pos[0] << " " << 0 << " " << pos[2] << " 32" << std::endl;
    		parseGeneralCommand(skirmishAIId,tss.str());

    		std::stringstream tss2;
    		tss2 << "cmd build " << unitId << " 392 32" << std::endl;
    		parseGeneralCommand(skirmishAIId,tss2.str());
    	}


    }else if(cmdid == "ping"){
		sendMsg(skirmishAIId,"pong!");
    }

    std::cout << "COMMAND FINISHED!" << std::endl;
    return true;
}






// enum UnitCommandOptions {
// 	UNIT_COMMAND_OPTION_INTERNAL_ORDER    = (1 << 3), //   8
// 	UNIT_COMMAND_OPTION_RIGHT_MOUSE_KEY   = (1 << 4), //  16
// 	UNIT_COMMAND_OPTION_SHIFT_KEY         = (1 << 5), //  32
// 	UNIT_COMMAND_OPTION_CONTROL_KEY       = (1 << 6), //  64
// 	UNIT_COMMAND_OPTION_ALT_KEY           = (1 << 7), // 128
// };
