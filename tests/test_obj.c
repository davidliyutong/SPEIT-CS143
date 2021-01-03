////
//// Created by 厉宇桐 on 2020/12/29.
////
//
//#include <stdio.h>
//#include "../common/object.h"
//#include "../common/stack.h"
//#include "../runtime/env.h"
//
//int main() {
//    lac_object_t iRes;
//    lac_object_t obj1 = env_create_lac_int(NULL, 0);
//    lac_object_t obj2 = env_create_lac_func("dup",0, 0, VM_FUNC_BASIC);
//    char str1[] = "Bonjour";
//    lac_object_t obj3 = env_create_lac_vec(NULL, (int) strlen(str1), 1, (void *) &str1);
//    lac_stack_t test_stack;
//    stack_init(&test_stack);
//
//    stack_push_data(&test_stack, &obj1);
//    stack_push_data(&test_stack, &obj1);
//    stack_push_data(&test_stack, &obj1);
//    stack_push_data(&test_stack, &obj2);
//    stack_push_data(&test_stack, &obj3);
//
//    while (!stack_is_empty(&test_stack)) {
//        iRes = stack_pop_data(&test_stack);
//        printf("%d\n", iRes.iRefCnt);
//    }
//
//    return 0;
//}