#include "test_device.h"
#include "test_context.h"

#include <stdlib.h>
#include <stdio.h>

static Suite *device_suite(void)
{
    Suite *s = suite_create("device");
    suite_add_tcase(s, cl_device_tcase_create());
    return s;
}

static Suite *context_suite(void)
{
    Suite *s = suite_create("context");
    suite_add_tcase(s, cl_context_tcase_create());
    return s;
}

int main(int argc, char **argv)
{
    int n_failed_tests;
    Suite *s = NULL;

    if (argc < 2) {
        printf("there is not enough arguments");
        return EXIT_FAILURE;
    }

    if (!strcmp("device",argv[1])) {
        s = device_suite();
    } else if (!strcmp("context", argv[1])){
        s = context_suite();
    } else {
        printf("test case does not exist");
        return EXIT_FAILURE;
    }

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);

    n_failed_tests = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (n_failed_tests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
