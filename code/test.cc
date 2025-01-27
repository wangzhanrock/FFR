#include <gtest/gtest.h>
#include "flexible_body_linear_external_ffr.h"
//TOOO:  add the lib and include into this project.


TEST(flexibleBodyLinearExternalFFRTest, readFEMData){
	FlexibleBodyLinearExternalFFR* pc = new FlexibleBodyLinearExternalFFR("test", 1);

	pc->readFEMData();

	EXPECT_EQ(pc->readFEMData();,0 )

}




//1. If you really need to check that a private variable has a particular characteristic, consider using assert() rather than trying to write a unit test for it.

//2. write getU0 and getM() inside the class

//3. compile with flag

//4.#define private public. It's just in unit tests,

//5.If you're only using GCC, you can use the compiler option -fno-access-control while compiling your unit tests.
//This will cause GCC to skip all access checks, but still keep the class layout the same.


//6. use frinend class

//class Working {
//    // ...
//    private:
//    int m_variable;
//
//#ifdef UNIT_TESTING
//    friend class TestBase;
//#endif
//};


http://scicomp.stackexchange.com/questions/4688/regression-testing-of-chaotic-numerical-models

http://cpptest.sourceforge.net/

http://www.americanscientist.org/issues/pub/wheres-the-real-bottleneck-in-scientific-computing

http://conference.scipy.org/scipy2013/tutorial_detail.php?id=106
