/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include <Types/Array.h>
#include <Types/String.h>

enum ECharacterType
{
	None,
	Word,
	Colon,
	SemiColon,
	Comma,
	OpenBrace,
	ClosedBrace,
	EndOfFile
};

struct FoundScriptData
{
	String name;

	String path;

	String full;
};

struct Utils
{
	static void ListDirectory(String* name, Array<FoundScriptData>& container);

	static ECharacterType GetCharacterType(const char*& str);

	static void SkipCharacterType(const char*& str, ECharacterType type);

	static String GetWord(const char*& in, bool bStep);

	static void SkipWord(const char*& in);

	static bool IsSpace(const char* buffer);
};