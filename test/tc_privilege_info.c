#include <stdio.h>
#include <privilege_information.h>

#define BRIGHTNESS 0
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BG_BLACK 40

static int fail_cnt = 0;
static int success_cnt = 0;

static void __change_color_to_red()
{
	printf("%c[%d;%dm", 0x1B, BRIGHTNESS, RED);
}

static void __change_color_to_green()
{
	printf("%c[%d;%dm", 0x1B, BRIGHTNESS, GREEN);
}

static void __change_color_to_yellow()
{
	printf("%c[%d;%dm", 0x1B, BRIGHTNESS, YELLOW);
}

static void __change_color_to_origin()
{
	printf("%c[%dm", 0x1B, 0);
}

static const char* __get_result_string(privilege_info_error_e ret)
{
	if (ret == PRVINFO_ERROR_NONE)
		return "PRVINFO_ERROR_NONE";
	else if (ret == PRVINFO_ERROR_INVALID_PARAMETER)
		return "PRVINFO_ERROR_INVALID_PARAMETER";
	else if (ret == PRVINFO_ERROR_OUT_OF_MEMORY)
		return "PRVINFO_ERROR_OUT_OF_MEMORY";
	else if (ret == PRVINFO_ERROR_INTERNAL_ERROR)
		return "PRVINFO_ERROR_INTERNAL_ERROR";
}

static void __check_get_privilege_display_name_result(privilege_info_error_e expected_result, privilege_info_error_e result, const char* display_name)
{
	printf("expected result = %s, result = %s\n", __get_result_string(expected_result), __get_result_string(result));

	if (expected_result != result) {
		printf("not matched\n");
		__change_color_to_red();
		printf("test fail\n");
		fail_cnt++;
	} else {
		printf("matched\n");
		if (result == PRVINFO_ERROR_NONE) {
			if (display_name == NULL) {
				printf("display_name must not be NULL\n");
				__change_color_to_red();
				printf("test fail\n");
				fail_cnt++;
				__change_color_to_origin();
				return;
			} else {
				printf("display_name = %s\n", display_name);
			}
		} else {
			if (display_name != NULL) {
				printf("display_name = %s\n", display_name);
				printf("display_name must be NULL\n");
				__change_color_to_red();
				printf("test fail\n");
				fail_cnt++;
				__change_color_to_origin();
				return;
			}
		}
		__change_color_to_green();
		printf("test success\n");
		success_cnt++;
	}
	__change_color_to_origin();
}


static void __check_get_privilege_description_result(privilege_info_error_e expected_result, privilege_info_error_e result, const char* description)
{
	printf("expected result = %s, result = %s\n", __get_result_string(expected_result), __get_result_string(result));

	if (expected_result != result) {
		printf("not matched\n");
		__change_color_to_red();
		printf("test fail\n");
		fail_cnt++;
	} else {
	   printf("matched\n");
	   if (result == PRVINFO_ERROR_NONE) {
			if (description == NULL) {
				printf("description must not be NULL\n");
				__change_color_to_red();
				printf("test fail\n");
				fail_cnt++;
				__change_color_to_origin();
				return;
			} else {
				printf("description = %s\n", description);
			}
		} else {
			if (description != NULL) {
				printf("description = %s\n", description);
				printf("description must be NULL\n");
				__change_color_to_red();
				printf("test fail\n");
				fail_cnt++;
				__change_color_to_origin();
				return;
			}
		}

		__change_color_to_green();
		printf("test success\n");
		success_cnt++;
	}
	__change_color_to_origin();
}

static void __test_privilege_info_get_display_name()
{
	int ret;
	char* display_name = NULL;

	printf("-----------------------------------------------------------\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/window.priority.set\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	ret = privilege_info_get_display_name("2.3", "http://tizen.org/privilege/window.priority.set", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_NONE, ret, display_name);

	printf("-----------------------------------------------------------\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/mediacapture\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (display_name != NULL) {
		free(display_name);
		display_name = NULL;
	}
	ret = privilege_info_get_display_name("2.3", "http://tizen.org/privilege/mediacapture", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_NONE, ret, display_name);

	printf("-----------------------------------------------------------\n");
	printf("Not existing privilege\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/RRRRRRRRRR\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (display_name != NULL) {
		free(display_name);
		display_name = NULL;
	}
	ret = privilege_info_get_display_name("2.3", "http://tizen.org/privilege/RRRRRRRRRR", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_NONE, ret, display_name);

	printf("-----------------------------------------------------------\n");
	printf("Invalid parameter\n");
	printf("api_version : NULL\n");
	printf("privilege : http://tizen.org/privilege/window.priority.set\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (display_name != NULL) {
		free(display_name);
		display_name = NULL;
	}
	ret = privilege_info_get_display_name(NULL, "http://tizen.org/privilege/window, priority.set", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_INVALID_PARAMETER, ret, display_name);

	free(display_name);
}
static void __test_privilege_info_get_display_name_by_pkgtype()
{
	int ret;
	char* display_name = NULL;

	printf("-----------------------------------------------------------\n");
	printf("pkgtype : PRVINFO_PACKAGE_TYPE_NATIVE\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/window.priority.set\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	ret = privilege_info_get_display_name_by_pkgtype("PRVINFO_PACKAGE_TYPE_NATIVE", "2.3", "http://tizen.org/privilege/window.priority.set", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_NONE, ret, display_name);

	printf("-----------------------------------------------------------\n");
	printf("pkgtype : PRVINFO_PACKAGE_TYPE_WEB\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/mediacapture\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (display_name != NULL) {
		free(display_name);
		display_name = NULL;
	}
	ret = privilege_info_get_display_name_by_pkgtype("PRVINFO_PACKAGE_TYPE_WEB", "2.3", "http://tizen.org/privilege/mediacapture", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_NONE, ret, display_name);

	printf("-----------------------------------------------------------\n");
	printf("Mismatched package type: write WEB as NATIVE\n");
	printf("pkgtype : PRVINFO_PACKAGE_TYPE_NATIVE\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/mediacapture\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (display_name != NULL) {
		free(display_name);
		display_name = NULL;
	}
	ret = privilege_info_get_display_name_by_pkgtype("PRVINFO_PACKAGE_TYPE_NATIVE", "2.3", "http://tizen.org/privilege/mediacapture", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_NONE, ret, display_name);


	printf("-----------------------------------------------------------\n");
	printf("Not existing privilege\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/RRRRRRRRRR\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (display_name != NULL) {
		free(display_name);
		display_name = NULL;
	}
	ret = privilege_info_get_display_name_by_pkgtype("PRVINFO_PACKAGE_TYPE_WEB", "2.3", "http://tizen.org/privilege/RRRRRRRRRR", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_NONE, ret, display_name);

	printf("-----------------------------------------------------------\n");
	printf("Invalid parameter\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/mediacapture\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (display_name != NULL) {
		free(display_name);
		display_name = NULL;
	}
	ret = privilege_info_get_display_name_by_pkgtype(NULL, "2.3", "http://tizen.org/privilege/mediacapture", &display_name);
	__check_get_privilege_display_name_result(PRVINFO_ERROR_INVALID_PARAMETER, ret, display_name);

	free(display_name);
}

static void __test_privilege_info_get_description()
{
	int ret;
	char* description = NULL;

	printf("-----------------------------------------------------------\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/window.priority.set\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	ret = privilege_info_get_description("2.3", "http://tizen.org/privilege/window.priority.set", &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_NONE, ret, description);

	printf("-----------------------------------------------------------\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/mediacapture\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (description != NULL) {
		free(description);
		description = NULL;
	}
	ret = privilege_info_get_description("2.3", "http://tizen.org/privilege/mediacapture", &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_NONE, ret, description);

	printf("-----------------------------------------------------------\n");
	printf("Not existing privilege\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/RRRRRRRRRR\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (description != NULL) {
		free(description);
		description = NULL;
	}
	ret = privilege_info_get_description("2.3", "http://tizen.org/privilege/RRRRRRRRRR", &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_NONE, ret, description);

	printf("-----------------------------------------------------------\n");
	printf("Invalid parameter\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/mediacapture\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (description != NULL) {
		free(description);
		description = NULL;
	}
	ret = privilege_info_get_description(NULL, "http://tizen.org/privilege/mediacapture", &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_INVALID_PARAMETER, ret, description);

	free(description);
}

static void __test_privilege_info_get_description_by_pkgtype()
{
	int ret;
	char* description = NULL;

	printf("-----------------------------------------------------------\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/window.priority.set\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	ret = privilege_info_get_description_by_pkgtype("PRVINFO_PACKAGE_TYPE_NATIVE", "2.3", "http://tizen.org/privilege/window.priority.set", &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_NONE, ret, description);

	printf("-----------------------------------------------------------\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/mediacapture\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (description != NULL) {
		free(description);
		description = NULL;
	}
	ret = privilege_info_get_description_by_pkgtype("PRVINFO_PACKAGE_TYPE_WEB", "2.3", "http://tizen.org/privilege/mediacapture", &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_NONE, ret, description);

	printf("-----------------------------------------------------------\n");
	printf("Mismatched package type: write WEB as NATIVE\n");
	printf("pkgtype : PRVINFO_PACKAGE_TYPE_NATIVE\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/mediacapture\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (description != NULL) {
		free(description);
		description = NULL;
	}
	ret = privilege_info_get_description_by_pkgtype("PRVINFO_PACKAGE_TYPE_NATIVE", "2.3", "http://tizen.org/privilege/mediacapture", &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_NONE, ret, description);

	printf("-----------------------------------------------------------\n");
	printf("Not existing privilege\n");
	printf("api_version : 2.3\n");
	printf("privilege : http://tizen.org/privilege/RRRRRRRRRR\n");
	printf("expected result : PRVINFO_ERROR_NONE\n");
	if (description != NULL) {
		free(description);
		description = NULL;
	}
	ret = privilege_info_get_description_by_pkgtype("PRVINFO_PACKAGE_TYPE_NATIVE", "2.3", "http://tizen.org/privilege/RRRRRRRRRR", &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_NONE, ret, description);

	printf("-----------------------------------------------------------\n");
	printf("Invalid parameter\n");
	printf("api_version : 2.3\n");
	printf("privilege : NULL\n");
	printf("expected result : PRVINFO_ERROR_INVALID_PARAMETER\n");
	if (description != NULL) {
		free(description);
		description = NULL;
	}
	ret = privilege_info_get_description_by_pkgtype("PRVINFO_PACKAGE_TYPE_NATIVE", "2.3", NULL, &description);
	__check_get_privilege_description_result(PRVINFO_ERROR_INVALID_PARAMETER, ret, description);

	free(description);
}

int main()
{
	__change_color_to_yellow();
	printf("Test function : privilege_info_get_display_name\n");
	__change_color_to_origin();
	__test_privilege_info_get_display_name();

	__change_color_to_yellow();
	printf("Test function : privilege_info_get_display_name_by_pkgtype\n");
	__change_color_to_origin();
	__test_privilege_info_get_display_name_by_pkgtype();

	__change_color_to_yellow();
	printf("Test function : privilege_info_get_description\n");
	__change_color_to_origin();
	__test_privilege_info_get_description();

	__change_color_to_yellow();
	printf("Test function : privilege_info_get_description_by_pkgtype\n");
	__change_color_to_origin();
	__test_privilege_info_get_description_by_pkgtype();

	__change_color_to_green();
	printf("Test Complete\n");
	printf("success : %d, ", success_cnt);
	__change_color_to_red();
	printf("fail : %d\n", fail_cnt);
	__change_color_to_origin();

}
