#pragma comment(lib, "gtest.lib")
#pragma comment(lib, "gtest_main.lib")
#pragma comment(lib, "gmock.lib")
#pragma comment(lib, "gmock_main.lib")

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

class DBConnectionInterface
{
public:
    DBConnectionInterface()
    {

    }
    virtual ~DBConnectionInterface()
    {

    }

    virtual const char* open(const char* nameOfDB) = 0;
    virtual bool close(const bool& value) = 0;
    virtual bool execQuery(const std::string& query) = 0;
};

class DBConnection : public DBConnectionInterface
{
public:
    DBConnection()
    {

    }
    ~DBConnection()
    {

    }

    const char* open(const char* nameOfDB) override
    {
        return nameOfDB;
    }
    bool close(const bool& value) override
    {
        if (value == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool execQuery(const std::string& query) override
    {
        return true;
    }
};

class ClassThatUsesDB
{
public:
    ClassThatUsesDB(DBConnectionInterface& dbInt) : _dbInt(dbInt)
    {

    }
    ~ClassThatUsesDB()
    {

    }

    const char* openConnection(const char* nameOfDB)
    {                     
        return _dbInt.open(nameOfDB);
    }                    
    bool useConnection(const std::string& query)
    {                     
        return _dbInt.execQuery(query);
    }                     
    bool closeConnection(const bool& value)
    {
        return _dbInt.close(value);
    }

    DBConnectionInterface& _dbInt;
};

class MockDBConnection : public DBConnectionInterface
{
public:
    MOCK_METHOD(const char*, open, (const char* nameOfDB), (override));
    MOCK_METHOD(bool, close, (const bool& value), (override));
    MOCK_METHOD(bool, execQuery, (const std::string& query), (override));
};

class SomeTestSuite : public ::testing::Test
{
public:
    void SetUp()
    {
        dbConn = new ClassThatUsesDB(mckObj);
    }

    void TearDown()
    {
        delete dbConn;
    }
    MockDBConnection mckObj;
    ClassThatUsesDB* dbConn;
};

TEST_F(SomeTestSuite, testCase1)
{
    const char* nameOfDB = "SimpleDB";

    EXPECT_CALL(mckObj, open(nameOfDB)).Times(1).WillOnce(::testing::Return("SimpleDB"));
    const char* result = dbConn->openConnection(nameOfDB);
    ASSERT_STREQ(nameOfDB, result);
}

TEST_F(SomeTestSuite, testCase2)
{
    std::string query = "SimpleQuery";

    EXPECT_CALL(mckObj, execQuery(query)).Times(1).WillOnce(::testing::Return(true));
    bool result = dbConn->useConnection(query);
    ASSERT_TRUE(result);
}

TEST_F(SomeTestSuite, testCase3)
{
    bool closeDB = true;

    EXPECT_CALL(mckObj, close(closeDB)).Times(1).WillOnce(::testing::Return(closeDB));
    int result = dbConn->closeConnection(closeDB);
    ASSERT_TRUE(result);
}

int main()
{
    ::testing::InitGoogleTest();

    return RUN_ALL_TESTS();
}
