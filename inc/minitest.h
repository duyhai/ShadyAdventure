//----------------------------------------------------------------------------
// minitest.h
//
// smallest possible (?) unit test framework
//
// Copyright (C) 2011 Neil Butterworth
//----------------------------------------------------------------------------

#ifndef INC_MINITEST_H
#define INC_MINITEST_H

#include <string>
#include <map>
#include <iostream>
#include <cstdlib>

namespace MiniTest {

struct Error {
	Error( const std::string & cond, const std::string & file, int line )
		: mCond( cond ), mFile( file ), mLine( line ) {
	}
	std::string mCond, mFile;
	int mLine;
};

typedef void(*TestFn)( void );

class Tester {

	public:

		static Tester & Instance( ) {
			static Tester instance;
			return instance;
		}

		void AddTest( const std::string & name, TestFn test ) {
			if ( ! mTests.insert( std::make_pair( name, test ) ).second ) { 
				std::cerr << "duplicate test name: " << name << std::endl;
				std::exit( -1 );
			}
		}

		void RunTests() {
			std::map<std::string,TestFn>::iterator it = mTests.begin();
			while ( it != mTests.end() ) {
				try {
					std::cout << it->first << " ... ";
					it->second();
					std::cerr << " passed\n";
				}
				catch( const Error & e ) {
					std::cerr << " failed [ " << e.mCond << " ] is false in " 
								<< e.mFile << " at line " << e.mLine << "\n";
					mErrors++;
				}
				++it;
			}
		}

		unsigned int  Errors() const {
			return mErrors;
		}

	private:
		
		Tester() : mErrors( 0 ) {
		}

		unsigned int mErrors;
		std::map <std::string, TestFn> mTests;
};

struct TestAdder {
	TestAdder( const std::string & name, TestFn fn ) {
		Tester::Instance().AddTest( name, fn );
	}
};

}  // end of namespace

#define MINI_MAIN() 								\
	{												\
		MiniTest::Tester::Instance().RunTests( );   \
		return MiniTest::Tester::Instance().Errors();   \
	}

#define MINI_TEST( testname )						\
	static void testname();							\
	static MiniTest::TestAdder TA_##testname( #testname, testname );   \
	void testname() 								

#define MINI_CHECK( cond )							\
	if ( ! (cond) ) {								\
		throw MiniTest::Error( #cond, __FILE__, __LINE__ );	\
	}

#endif

