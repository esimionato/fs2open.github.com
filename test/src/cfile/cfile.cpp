
#include <gtest/gtest.h>
#include <graphics/font.h>

#include "util/FSTestFixture.h"

class CFileInitTest : public test::FSTestFixture {
 public:
	CFileInitTest() : test::FSTestFixture(INIT_NONE) {
		pushModDir("cfile");
	}

 protected:
	virtual void SetUp() override {
		test::FSTestFixture::SetUp();
	}
	virtual void TearDown() override {
		test::FSTestFixture::TearDown();

		cfile_close();
	}
};

TEST_F(CFileInitTest, wrong_data_case) {
	SCP_string cfile_dir(TEST_DATA_PATH);
	cfile_dir += DIR_SEPARATOR_CHAR;
	cfile_dir += "test"; // Cfile expects something after the path

	ASSERT_FALSE(cfile_init(cfile_dir.c_str()));

	ASSERT_TRUE(cf_exists("ships.tbl", CF_TYPE_TABLES));
}

TEST_F(CFileInitTest, right_data_case) {
	SCP_string cfile_dir(TEST_DATA_PATH);
	cfile_dir += DIR_SEPARATOR_CHAR;
	cfile_dir += "test"; // Cfile expects something after the path

	ASSERT_FALSE(cfile_init(cfile_dir.c_str()));

	ASSERT_TRUE(cf_exists("ships.tbl", CF_TYPE_TABLES));
}

class CFileTest : public test::FSTestFixture {
 public:
	CFileTest() : test::FSTestFixture(INIT_CFILE) {
		pushModDir("cfile");
	}

 protected:
	virtual void SetUp() override {
		test::FSTestFixture::SetUp();
	}
	virtual void TearDown() override {
		test::FSTestFixture::TearDown();

		cfile_close();
	}
};

TEST_F(CFileTest, list_files_in_vps_and_dirs) {
	SCP_vector<SCP_string> table_files;
	ASSERT_EQ(4, cf_get_file_list(table_files, CF_TYPE_TABLES, "*", CF_SORT_NAME));

	ASSERT_EQ((size_t)4, table_files.size());

	ASSERT_STREQ("dir", table_files[0].c_str());
	ASSERT_STREQ("dir2", table_files[1].c_str());

	ASSERT_STREQ("test", table_files[2].c_str());
	ASSERT_STREQ("test2", table_files[3].c_str());

	table_files.clear();
	extern int Skip_packfile_search;
	Skip_packfile_search = 1;
	ASSERT_EQ(2, cf_get_file_list(table_files, CF_TYPE_TABLES, "*", CF_SORT_NAME));
	Skip_packfile_search = 0;

	ASSERT_EQ((size_t)2, table_files.size());

	ASSERT_STREQ("dir", table_files[0].c_str());
	ASSERT_STREQ("dir2", table_files[1].c_str());
}

TEST_F(CFileTest, test_get_path_type) {
	SCP_string cfile_dir(TEST_DATA_PATH);
	cfile_dir += DIR_SEPARATOR_CHAR;
	cfile_dir += "test"; // Cfile expects something after the path

	ASSERT_FALSE(cfile_init(cfile_dir.c_str()));

	// Test simple case
	ASSERT_EQ(CF_TYPE_DATA, cfile_get_path_type("data"));
	ASSERT_EQ(CF_TYPE_DATA, cfile_get_path_type("///data"));
	ASSERT_EQ(CF_TYPE_DATA, cfile_get_path_type("data\\\\\\"));
	ASSERT_EQ(CF_TYPE_DATA, cfile_get_path_type("////data\\\\\\"));

	// Test single subdirectory case
	ASSERT_EQ(CF_TYPE_INTERFACE, cfile_get_path_type("data/interface"));
	ASSERT_EQ(CF_TYPE_INTERFACE, cfile_get_path_type("///data/interface"));
	ASSERT_EQ(CF_TYPE_INTERFACE, cfile_get_path_type("data/interface\\\\\\"));
	ASSERT_EQ(CF_TYPE_INTERFACE, cfile_get_path_type("////data/interface\\\\\\"));

	// Test nested subdirectory case
	ASSERT_EQ(CF_TYPE_INTERFACE_MARKUP, cfile_get_path_type("data/interface/markup"));
	ASSERT_EQ(CF_TYPE_INTERFACE_MARKUP, cfile_get_path_type("///data/interface/markup"));
	ASSERT_EQ(CF_TYPE_INTERFACE_MARKUP, cfile_get_path_type("data/interface/markup\\\\\\"));
	ASSERT_EQ(CF_TYPE_INTERFACE_MARKUP, cfile_get_path_type("////data/interface/markup\\\\\\"));
}
