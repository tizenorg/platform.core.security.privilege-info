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

#define TryReturn(condition, returnValue, ...)  \
    if (!(condition)) { \
        LOGE(__VA_ARGS__); \
        return returnValue; \
    } \
    else {;}

#ifndef PI_API
#define PI_API __attribute__((visibility("default")))
#endif

typedef enum
{
    PRVINFO_ERROR_NO_MATCHING_PRIVILEGE        = TIZEN_ERROR_PRIVILEGE_INFORMATION | 0x01
}privilege_info_internal_error_e;

int privilege_info_get_display_name_string_id(const char* api_version, const char *privilege, char **display_name_string_id)
{
    TryReturn(api_version != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    char* temp = NULL;

    // Check NATIVE
    int ret = privilege_db_manager_get_privilege_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege, api_version, &temp);

    if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
        if(temp == NULL)
        {
            *display_name_string_id = NULL;
        }
        else if(strcmp(temp,"") == 0)
        {
            *display_name_string_id = NULL;
        }
        else
        {
            goto err_none;
        }
    }
    else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        goto err_internal_error;
    }

    if(temp != NULL)
    {
        free(temp);
        temp = NULL;
    }

    // Check WEB
    ret = privilege_db_manager_get_privilege_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT, privilege, api_version, &temp);

    if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
        if(temp == NULL)
        {
            goto err_no_matching_privilege;
        }
        else if(strcmp(temp,"") == 0)
        {
            goto err_no_matching_privilege;
        }
        else
        {
            goto err_none;
        }
    }
    else if(ret == PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        goto err_no_matching_privilege;
    }
    else
    {
        goto err_internal_error;
    }

err_none:
    *display_name_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
    TryReturn(*display_name_string_id != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation is failed.");
    memcpy(*display_name_string_id, temp, strlen(temp));
    LOGD("display_name_string_id = %s", *display_name_string_id);
    free(temp);
    return PRVINFO_ERROR_NONE;

err_no_matching_privilege:
    *display_name_string_id = NULL;
    free(temp);
    return PRVINFO_ERROR_NO_MATCHING_PRIVILEGE;

err_internal_error:
    free(temp);
    return PRVINFO_ERROR_INTERNAL_ERROR;
}


int privilege_info_get_display_name_by_string_id(const char *string_id, char **display_name)
{
    char *temp = NULL;
    TryReturn(string_id != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] string_id is NULL");

    temp = dgettext("privilege", string_id);

    *display_name = (char*)calloc(strlen(temp) + 1, sizeof(char));
    TryReturn(*display_name != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");

    memcpy(*display_name, temp, strlen(temp));

    return PRVINFO_ERROR_NONE;
}

PI_API
int privilege_info_get_display_name(const char* api_version, const char* privilege, char** display_name)
{
    int ret = 0;
    char* display_name_string_id = NULL;

    TryReturn(api_version != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    ret = privilege_info_get_display_name_string_id(api_version, privilege, &display_name_string_id);
    
    if(ret == PRVINFO_ERROR_NO_MATCHING_PRIVILEGE)
    {
        char tempPrivilege[256] = {0,};
        char* token = NULL;
        char* temp = NULL;
        memcpy(tempPrivilege, privilege, strlen(privilege));
        
        token = strtok(tempPrivilege, "/");
        while(token)
        {
            temp = token;
            token = strtok(NULL, "/");
        }
        *display_name = (char*)calloc(strlen(temp) + 1, sizeof(char));
        TryReturn(*display_name != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
        memcpy(*display_name, temp, strlen(temp));
    }
    else if(ret == PRVINFO_ERROR_NONE)
    {
        ret = privilege_info_get_display_name_by_string_id(display_name_string_id, display_name);
        free(display_name_string_id);
        TryReturn(ret == PRVINFO_ERROR_NONE, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
    }
    else
    {
        return PRVINFO_ERROR_INTERNAL_ERROR;
    }
    return PRVINFO_ERROR_NONE;
}


int privilege_info_get_description_string_id(const char* api_version, const char *privilege, char **description_string_id)
{
    TryReturn(api_version != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    char* temp = NULL;

    // Check NATIVE
    int ret = privilege_db_manager_get_privilege_description(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege, api_version, &temp);

    if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
        if(temp == NULL)
        {
            *description_string_id = NULL;
        }
        else if(strcmp(temp, "") == 0)
        {
            *description_string_id = NULL;
        }
        else
        {
            goto err_none;
        }
    }
    else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        goto err_internal_error;
    }

    if(temp != NULL)
    {
        free(temp);
        temp = NULL;
    }
    // Check WEB
    ret = privilege_db_manager_get_privilege_description(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT, privilege, api_version, &temp);

    if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
        if(temp == NULL)
        {
            goto err_no_matching_privilege;
        }
        else if(strcmp(temp,"") == 0)
        {
            goto err_no_matching_privilege;
        }
        else
        {
            goto err_none;
        }
    }
    else if(ret == PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        goto err_no_matching_privilege;
    }
    else
    {
        goto err_internal_error;
    }

err_none:
    *description_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
    TryReturn(*description_string_id != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation is failed.");
    memcpy(*description_string_id, temp, strlen(temp));
    LOGD("description_string_id = %s", *description_string_id);
    free(temp);
    return PRVINFO_ERROR_NONE;

err_no_matching_privilege:
    *description_string_id = NULL;
    free(temp);
    return PRVINFO_ERROR_NO_MATCHING_PRIVILEGE;

err_internal_error:
    free(temp);
    return PRVINFO_ERROR_INTERNAL_ERROR;
}

int privilege_info_get_description_by_string_id(const char *string_id, char **description)
{
    char *temp = NULL;
    TryReturn(string_id != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] string_id is NULL");

    temp = dgettext("privilege", string_id);

    *description = (char*)calloc(strlen(temp) + 1, sizeof(char));
    TryReturn(*description != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");

    memcpy(*description, temp, strlen(temp));

    return PRVINFO_ERROR_NONE;
}

PI_API
int privilege_info_get_description(const char* api_version, const char* privilege, char** description)
{
    int ret = 0;
    char* description_string_id = NULL;

    TryReturn(api_version != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    ret = privilege_info_get_description_string_id(api_version, privilege, &description_string_id);

    if(ret == PRVINFO_ERROR_NO_MATCHING_PRIVILEGE)
    {
        char* temp = NULL;
        temp = dgettext("privilege", "IDS_TPLATFORM_BODY_THIS_PRIVILEGE_IS_NOT_DEFINED");
        *description = (char*)calloc(strlen(temp) + 1, sizeof(char));
        TryReturn(*description != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");

        memcpy(*description, temp, strlen(temp));
    }
    else if(ret == PRVINFO_ERROR_NONE)
    {
        ret = privilege_info_get_description_by_string_id(description_string_id, description);
        free(description_string_id);
        TryReturn(ret == PRVINFO_ERROR_NONE, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
    }
    else
    {
        return PRVINFO_ERROR_INTERNAL_ERROR;
    }
    return PRVINFO_ERROR_NONE;
}

int privilege_info_get_display_name_string_id_by_pkgtype(const char* package_type, const char* api_version, const char *privilege, char **display_name_string_id)
{
    TryReturn(package_type != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] package_type is NULL");
    TryReturn(api_version != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    char* temp = NULL;

    // Check package_type
    int ret = 0;
    if(strcmp(package_type,"PRVINFO_PACKAGE_TYPE_NATIVE") == 0)
    {
        LOGD("package_type = %s", package_type);
    }
    else if(strcmp(package_type,"PRVINFO_PACKAGE_TYPE_WEB") == 0)
    {
        LOGD("package_type = %s", package_type);
        ret = privilege_db_manager_get_privilege_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT, privilege, api_version, &temp);
        if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
        {
            if(temp == NULL)
            {
                *display_name_string_id = NULL;
            }
            else if(strcmp(temp,"") == 0)
            {
                *display_name_string_id = NULL;
            }
            else
            {
                goto err_none;
            }
        }
        else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
        {
            goto err_internal_error;
        }

        if(temp != NULL)
        {
            free(temp);
            temp = NULL;
        }
    }
    else
    {
        LOGD("package_type = %s", package_type);
        free(temp);
        return PRVINFO_ERROR_INVALID_PARAMETER;
    }

    ret = privilege_db_manager_get_privilege_display(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege, api_version, &temp);

    if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
        if(temp == NULL)
        {
            goto err_no_matching_privilege;
        }
        else if(strcmp(temp, "") == 0)
        {
            goto err_no_matching_privilege;
        }
        else
        {
            goto err_none;
        }
    }
    else if(ret == PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        goto err_no_matching_privilege;
    }
    else
    {
        goto err_internal_error;
    }

err_none:
    *display_name_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
    TryReturn(*display_name_string_id != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation is failed.");
    memcpy(*display_name_string_id, temp, strlen(temp));
    LOGD("display_name_string_id = %s", *display_name_string_id);
    free(temp);
    return PRVINFO_ERROR_NONE;

err_no_matching_privilege:
    *display_name_string_id = NULL;
    free(temp);
    return PRVINFO_ERROR_NO_MATCHING_PRIVILEGE;

err_internal_error:
    free(temp);
    return PRVINFO_ERROR_INTERNAL_ERROR;
}

PI_API
int privilege_info_get_display_name_by_pkgtype(const const char* package_type, const char* api_version, const char* privilege, char** display_name)
{
    int ret = 0;
    char* display_name_string_id = NULL;

    TryReturn(package_type != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] package_type is NULL");
    TryReturn(api_version != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    ret = privilege_info_get_display_name_string_id_by_pkgtype(package_type, api_version, privilege, &display_name_string_id);

    if(ret == PRVINFO_ERROR_NO_MATCHING_PRIVILEGE)
    {
        char tempPrivilege[256] = {0,};
        char* token = NULL;
        char* temp = NULL;
        memcpy(tempPrivilege, privilege, strlen(privilege));
        token = strtok(tempPrivilege, "/");
        while(token)
        {
            temp = token;
            token = strtok(NULL, "/");
        }
        *display_name = (char*)calloc(strlen(temp) + 1, sizeof(char));
        TryReturn(*display_name != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
        memcpy(*display_name, temp, strlen(temp));
    }
    else if(ret == PRVINFO_ERROR_NONE)
    {
        ret = privilege_info_get_display_name_by_string_id(display_name_string_id, display_name);
        free(display_name_string_id);
        TryReturn(ret == PRVINFO_ERROR_NONE, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
    }
    else
    {
        return PRVINFO_ERROR_INTERNAL_ERROR;
    }
    return PRVINFO_ERROR_NONE;
}


int privilege_info_get_description_string_id_by_pkgtype(const char* package_type, const char* api_version, const char *privilege, char **description_string_id)
{
    TryReturn(package_type != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] package_type is NULL");
    TryReturn(api_version != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    char* temp = NULL;

    // Check package_type
    int ret = 0;

    if(strcmp(package_type,"PRVINFO_PACKAGE_TYPE_WEB") == 0)
    {
        LOGD("package_type = %s", package_type);
        ret = privilege_db_manager_get_privilege_description(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_WRT, privilege, api_version, &temp);
        if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
        {
            if(temp == NULL)
            {
                *description_string_id = NULL;
            }
            else if(strcmp(temp,"") == 0)
            {
                *description_string_id = NULL;
            }
            else
            {
                goto err_none;
            }
        }
        else if(ret != PRIVILEGE_DB_NO_EXIST_RESULT)
        {
            goto err_internal_error;
        }

        if(temp != NULL)
        {
            free(temp);
            temp = NULL;
        }
    }
    else if(strcmp(package_type,"PRVINFO_PACKAGE_TYPE_NATIVE") == 0)
    {
        LOGD("package_type = %s", package_type);
    }
    else
    {
        LOGD("package_type = %s", package_type);
        free(temp);
        return PRVINFO_ERROR_INVALID_PARAMETER;
    }

    ret = privilege_db_manager_get_privilege_description(PRIVILEGE_DB_MANAGER_PACKAGE_TYPE_CORE, privilege, api_version, &temp);

    if(ret == PRIVILEGE_DB_MANAGER_ERR_NONE)
    {
        if(temp == NULL)
        {
            goto err_no_matching_privilege;
        }
        else if(strcmp(temp,"") == 0)
        {
            goto err_no_matching_privilege;
        }
        else
        {
            goto err_none;
        }
    }
    else if(ret == PRIVILEGE_DB_NO_EXIST_RESULT)
    {
        goto err_no_matching_privilege;
    }
    else
    {
        goto err_internal_error;
    }


err_none:
    *description_string_id = (char*)calloc(strlen(temp) + 1, sizeof(char));
    TryReturn(*description_string_id != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation is failed.");
    memcpy(*description_string_id, temp, strlen(temp));
    LOGD("description_string_id = %s", *description_string_id);
    free(temp);
    return PRVINFO_ERROR_NONE;

err_no_matching_privilege:
    *description_string_id = NULL;
    free(temp);
    return PRVINFO_ERROR_NO_MATCHING_PRIVILEGE;

err_internal_error:
    free(temp);
    return PRVINFO_ERROR_INTERNAL_ERROR;
}

PI_API
int privilege_info_get_description_by_pkgtype(const char* package_type, const char* api_version, const char* privilege, char** description)
{
    int ret = 0;
    char* description_string_id = NULL;

    TryReturn(package_type != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] package_type is NULL");
    TryReturn(api_version != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] api_version is NULL");
    TryReturn(privilege != NULL, PRVINFO_ERROR_INVALID_PARAMETER, "[PRVINFO_ERROR_INVALID_PARAMETER] privilege is NULL");

    ret = privilege_info_get_description_string_id_by_pkgtype(package_type, api_version, privilege, &description_string_id);

    if(ret == PRVINFO_ERROR_NO_MATCHING_PRIVILEGE)
    {
        char* temp = NULL;
        temp = dgettext("privilege", "IDS_TPLATFORM_BODY_THIS_PRIVILEGE_IS_NOT_DEFINED");
        *description = (char*)calloc(strlen(temp) + 1, sizeof(char));
        TryReturn(*description != NULL, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
        memcpy(*description, temp, strlen(temp));
        //return PRVINFO_ERROR_NO_MATCHING_PRIVILEGE;
    }
    else if(ret == PRVINFO_ERROR_NONE)
    {
        ret = privilege_info_get_description_by_string_id(description_string_id, description);
        free(description_string_id);
        TryReturn(ret == PRVINFO_ERROR_NONE, PRVINFO_ERROR_OUT_OF_MEMORY, "[PRVINFO_ERROR_OUT_OF_MEMORY] Memory allocation failed.");
    }
    else
    {
        return PRVINFO_ERROR_INTERNAL_ERROR;
    }
    return PRVINFO_ERROR_NONE;
}

