#include "Unity/unity.h"
#include "adc_to_celsius.h"

void setUp(){};

void tearDown(){};

void test_de_float(){
	TEST_ASSERT_FLOAT_WITHIN(0.1,27,adc_to_celsius(876.081));
}

int main(void){
	UNITY_BEGIN();
	RUN_TEST(test_de_float);
	return UNITY_END();
}
