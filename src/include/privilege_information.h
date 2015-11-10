/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_SECURITY_PRIVILEGE_INFO_H
#define __TIZEN_SECURITY_PRIVILEGE_INFO_H

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SECURITY_FRAMEWORK_PRIVILEGE_INFO_MODULE
 * @{
 */

/**
 * @brief Enumerations of error code for Privilege Information.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 */
typedef enum
{
    PRVINFO_ERROR_NONE                         = TIZEN_ERROR_NONE, /**< Successful */
    PRVINFO_ERROR_INVALID_PARAMETER            = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid function parameter */
    PRVINFO_ERROR_OUT_OF_MEMORY                = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
    PRVINFO_ERROR_INTERNAL_ERROR               = TIZEN_ERROR_UNKNOWN, /**< Unknown error */
} privilege_info_error_e;


/**
 * @brief Gets the display name of the given privilege.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks @a display_name must be released with free() by you.
 * @param [in] api_version The version of API
 * @param [in] privilege The privilege
 * @param [out] display_name The display name of the privilege
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 */
int privilege_info_get_display_name(const char* api_version, const char* privilege, char **display_name);

/**
 * @brief Gets the display name of the given privilege.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks @a display_name must be released with free() by you.
 * @remarks package_type must be one of followings: PRVINFO_PACKAGE_TYPE_NATIVE, PRVINFO_PACKAGE_TYPE_WEB
 * @param [in] package_type The type of application package
 * @param [in] api_version The version of API
 * @param [in] privilege The privilege
 * @param [out] display_name The display name of the privilege
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 */
int privilege_info_get_display_name_by_pkgtype(const char* package_type, const char* api_version, const char* privilege, char **display_name);

/**
 * @brief Gets the description of the given privilege.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks @a description must be released with free() by you.
 * @param [in] api_version The version of API
 * @param [in]  privilege The privilege
 * @param [out] description The description of the privilege
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 */
int privilege_info_get_description(const char* api_version, const char *privilege, char **description);

/**
 * @brief Gets the description of the given privilege.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks @a description must be released with free() by you.
 * @remarks package_type must be one of followings: PRVINFO_PACKAGE_TYPE_NATIVE, PRVINFO_PACKAGE_TYPE_WEB 
 * @param [in] package_type The type of application package
 * @param [in] api_version The version of API
 * @param [in]  privilege The privilege
 * @param [out] description The description of the privilege
 * @return 0 on success, otherwise a negative error value.
 * @retval #PRVINFO_ERROR_NONE Successful                                                                                                                                            
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 */
int privilege_info_get_description_by_pkgtype(const char* package_type, const char* api_version, const char *privilege, char **description);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_SECURITY_PRIVILEGE_INFO_H
