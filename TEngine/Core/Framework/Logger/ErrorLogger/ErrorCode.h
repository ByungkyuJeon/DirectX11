#pragma once

#include <string>

enum ErrorCode
{
	INVALID_ERROR = 0,

	// for memory pools
	OUT_OF_MEMORY = 1,
	EXISTING_OBJECT = 2,
	NOT_EXISTING_OBJECT = 3,


	END
};

static std::string ErrorCodeToString(ErrorCode InErrorCode)
{
	std::string errorStr = "";

	switch (InErrorCode)
	{
	case INVALID_ERROR:
	case END:
		break;
	case OUT_OF_MEMORY:
		errorStr = "ERROR CODE : " + std::to_string(OUT_OF_MEMORY) + " memory pool out of memory";
		break;
	case EXISTING_OBJECT:
		errorStr = "ERROR CODE : " + std::to_string(EXISTING_OBJECT) + " already existing memory object";
		break;
	case NOT_EXISTING_OBJECT:
		errorStr = "ERROR CODE : " + std::to_string(NOT_EXISTING_OBJECT) + " not existing memory object";
		break;
	default:
		break;
	}

	return "";
}