/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include <Types/Array.h>
#include <Types/String.h>

enum ECharacterType
{
	None,
	Word,
	Colon,
	Comma,
	OpenBrace,
	ClosedBrace,
	EndOfFile
};

struct Utils
{
	static void ListDirectory(String* name, Array<String>& container);

	static ECharacterType GetCharacterType(const char* str);

	static void SkipCharacterType(const char*& str, ECharacterType type);

	static bool Expected(const char*& in, const char* word, bool b);

	static bool IsSpace(const char* str);
};