#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>
#include <dlog.h>
#include <privilege_db_manager.h>
#include "privilege_information.h"
#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "PRIVILEGE_INFO"
#endif

#define TryReturn(condition, expr, returnValue, ...)  \
	if (!(condition)) { \
		expr; \
		LOGE(__VA_ARGS__); \
		return returnValue; \
	}

#ifndef PI_API
#define PI_API __attribute__((visibility("default")))
#endif

typedef enum {
	PRVINFO_ERROR_NO_MATCHING_PRIVILEGE		= TIZEN_ERROR_PRIVILEGE_INFORMATION | 0x01
} privilege_info_internal_error_e;

int privilege_info_get_string_id(const char* package_type_string, int display, const char* api_version, const char *privilege, char **string_id)
{
    TryReturn(api_version != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

	char* temp = NULL;
    int ret;
	privilege_db_manager_package_type_e package_type;

	if (package_type_string != NULL)
		goto get_string_id_with_package_type;

    /* Check NATIVE */
    if (display)
        ret = privilege_db_manager_get_privilege_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege, api_version, &temp);
    else
        ret = privilege_db_manager_get_privilege_description(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege, api_version, &temp);

    if (ret == PRIVILEGE_DB_MANAGER_ERR_NONE) {
        goto err_none;
    } else if (ret != PRIVILEGE_DB_NO_EXIST_RESULT) {
        goto err_internal_error;
    }
	/* Check WEB */

get_string_id_with_package_type:
	if (package_type_string == NULL || strcmp(package_type_string, "PRVINFO_PACKAGE_TYPE_WEB") == 0)
        package_type = PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT;
	else if (strcmp(package_type_string, "PRVINFO_PACKAGE_TYPE_NATIVE") == 0)
        package_type = PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE;
	else
        return PRVINFO_ERROR_INVALID_PARAMETER;

    if (display)
        ret = privilege_db_manager_get_privilege_display(package_type, privilege, api_version, &temp);
    else
        ret = privilege_db_manager_get_privilege_description(package_type, privilege, api_version, &temp);


    if (ret == PRIVILEGE_DB_MANAGER_ERR_NONE) {
        goto err_none;
    } else if (ret == PRIVILEGE_DB_NO_EXIST_RESULT) {
        goto err_no_matching_privilege;
    } else {
        goto err_internal_error;
    }

err_none:
    *string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
    TryReturn(*string_id != NULL, free(temp), PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation is failed.");
    memcpy(*string_id, temp, strlen(temp));
    free(temp);
    return PRVINFO_ERROR_NONE;

err_no_matching_privilege:
    *string_id = NULL;
    free(temp);
    return PRVINFO_ERROR_NO_MATCHING_PRIVILEGE;

err_internal_error:
    free(temp);
    return PRVINFO_ERROR_INTERNAL_ERROR;
}

int privilege_info_get_string_by_string_id(const char *string_id, char **string)
{
	char *temp = NULL;
	TryReturn(string_id != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] string_id is NULL");

	temp = dgettext("privilege", string_id);

	*string = strdup(temp);
	TryReturn(*string != NULL, , PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] strdup of string failed.");

	return PRVINFO_ERROR_NONE;
}

PI_API
int privilege_info_get_display_name(const char* api_version, const char* privilege, char** display_name)
{
	int ret = 0;
	char* string_id = NULL;

	TryReturn(api_version != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
	TryReturn(privilege != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

	ret = privilege_info_get_string_id(NULL, 1, api_version, privilege, &string_id);

	if (ret == PRVINFO_ERROR_NO_MATCHING_PRIVILEGE) {
		char* tempPrivilege = NULL;
		char* token = NULL;
		char* temp = NULL;
		char* save = NULL;

		tempPrivilege = strdup(privilege);
		TryReturn(tempPrivilege != NULL, free(tempPrivilege), PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] tempPrivilege's strdup is failed.");

		token = strtok_r(tempPrivilege, "/", &save);
		while (token) {
			temp = token;
			token = strtok_r(NULL, "/", &save);
		}
		*display_name = (char*)calloc(strlen(temp) + 1, sizeof(char));
		TryReturn(*display_name != NULL, free(tempPrivilege), PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
		memcpy(*display_name, temp, strlen(temp));
		free(tempPrivilege);
	} else if (ret == PRVINFO_ERROR_NONE) {
		ret = privilege_info_get_string_by_string_id(string_id, display_name);
		free(string_id);
		TryReturn(ret == PRVINFO_ERROR_NONE, , PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
	} else {
		return PRVINFO_ERROR_INTERNAL_ERROR;
	}
	return PRVINFO_ERROR_NONE;
}


PI_API
int privilege_info_get_description(const char* api_version, const char* privilege, char** description)
{
	int ret = 0;
	char* string_id = NULL;

	TryReturn(api_version != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
	TryReturn(privilege != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

	ret = privilege_info_get_string_id(NULL, 0, api_version, privilege, &string_id);

	if (ret == PRVINFO_ERROR_NO_MATCHING_PRIVILEGE) {
		ret = privilege_info_get_string_by_string_id("IDS_TPLATFORM_BODY_THIS_PRIVILEGE_IS_NOT_DEFINED", description);
		TryReturn(ret == PRVINFO_ERROR_NONE, , PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
	} else if (ret == PRVINFO_ERROR_NONE) {
		ret = privilege_info_get_string_by_string_id(string_id, description);
		free(string_id);
		TryReturn(ret == PRVINFO_ERROR_NONE, , PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
	} else {
		return PRVINFO_ERROR_INTERNAL_ERROR;
	}
	return PRVINFO_ERROR_NONE;
}

PI_API
int privilege_info_get_display_name_by_pkgtype(const const char* package_type, const char* api_version, const char* privilege, char** display_name)
{
	int ret = 0;
	char* string_id = NULL;

	TryReturn(package_type != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] package_type is NULL");
	TryReturn(api_version != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
	TryReturn(privilege != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

	ret = privilege_info_get_string_id(package_type, 1, api_version, privilege, &string_id);
	TryReturn(ret != PRVINFO_ERROR_INVALID_PARAMETER, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] invalid package_type : %s", package_type);

	if (ret == PRVINFO_ERROR_NO_MATCHING_PRIVILEGE) {
		char* tempPrivilege = NULL;
		char* token = NULL;
		char* temp = NULL;
		char* save = NULL;
		tempPrivilege = strdup(privilege);
		TryReturn(tempPrivilege != NULL, free(tempPrivilege), PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] tempPrivilege's strdup is failed.");
		token = strtok_r(tempPrivilege, "/", &save);
		while (token) {
			temp = token;
			token = strtok_r(NULL, "/", &save);
		}
		*display_name = (char*)calloc(strlen(temp) + 1, sizeof(char));
		TryReturn(*display_name != NULL, free(tempPrivilege), PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
		memcpy(*display_name, temp, strlen(temp));
		free(tempPrivilege);
	} else if (ret == PRVINFO_ERROR_NONE) {
		ret = privilege_info_get_string_by_string_id(string_id, display_name);
		free(string_id);
		TryReturn(ret == PRVINFO_ERROR_NONE, , PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
	} else {
		return PRVINFO_ERROR_INTERNAL_ERROR;
	}
	return PRVINFO_ERROR_NONE;
}

PI_API
int privilege_info_get_description_by_pkgtype(const char* package_type, const char* api_version, const char* privilege, char** description)
{
	int ret = 0;
	char* string_id = NULL;

	TryReturn(package_type != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] package_type is NULL");
	TryReturn(api_version != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
	TryReturn(privilege != NULL, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    ret = privilege_info_get_string_id(package_type, 0, api_version, privilege, &string_id);
	TryReturn(ret != PRVINFO_ERROR_INVALID_PARAMETER, , PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] invalid package_type : %s", package_type);

	if (ret == PRVINFO_ERROR_NO_MATCHING_PRIVILEGE) {
		ret = privilege_info_get_string_by_string_id("IDS_TPLATFORM_BODY_THIS_PRIVILEGE_IS_NOT_DEFINED", description);
		TryReturn(ret == PRVINFO_ERROR_NONE, , PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
	} else if (ret == PRVINFO_ERROR_NONE) {
		ret = privilege_info_get_string_by_string_id(string_id, description);
		free(string_id);
		TryReturn(ret == PRVINFO_ERROR_NONE, , PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
	} else {
		return PRVINFO_ERROR_INTERNAL_ERROR;
	}
	return PRVINFO_ERROR_NONE;
}

