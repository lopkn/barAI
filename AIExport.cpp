/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "AIExport.h"
#include "AISCommands.h"
#include "ExternalAI/Interface/AISEvents.h"


#include <stdexcept>
#include <map>
#include <iostream>


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

	int ret = 0;

	return ret; // (ret != 0) => error
}

EXPORT(int) release(int skirmishAIId) {


	int ret = 0;

	return ret; // (ret != 0) => error
}

EXPORT(int) handleEvent(int skirmishAIId, int topic, const void* data) {

	//			if(myCallback != nullptr){
	//			   std::cout << "myCallback initted" << std::endl;
	//		    }

	switch (topic) {
	    case EventTopic::EVENT_UPDATE: {
	    	const SUpdateEvent* event = static_cast<const SUpdateEvent*>(data);
	    	int frame = event->frame;
	    	// std::cout << "frame: " << frame << std::endl; // works -l
	        
	        break;
	    }
		case EventTopic::EVENT_UNIT_CREATED: {
			const SUnitCreatedEvent* event = static_cast<const SUnitCreatedEvent*>(data);
			int unit = event->unit;
			int builder = event->builder;
			std::cout << "unit "<< unit << " created by " << builder << std::endl;

			if(builder == -1){
				// unit is created by engine
			}


			break;
		}
		case EventTopic::EVENT_UNIT_FINISHED: {
			const SUnitFinishedEvent* event = static_cast<const SUnitFinishedEvent*>(data);
			int unit = event->unit;
			std::cout << "unit "<< unit << " finished" << std::endl;


			const char* text = "/say HEYYYYYYYO";
			SSendTextMessageCommand cmd = {text, 0};




			myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_SEND_TEXT_MESSAGE, &cmd);



			//try to move

			float pos[3] = {50.0f,50.0f,50.0f};
			SMoveUnitCommand cmd2 = {};
			cmd2.unitId = unit;
			cmd2.options = 0;
			cmd2.toPos_posF3 = pos;

			myCallback->Engine_handleCommand(skirmishAIId, COMMAND_TO_ID_ENGINE, -1, COMMAND_UNIT_FIGHT, &cmd2);


			break;
		}


	    default:
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
