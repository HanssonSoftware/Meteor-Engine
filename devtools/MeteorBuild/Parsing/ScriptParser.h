/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
class Module;

enum class TokenType
{
	Identifier,         // Project, IncludePath, Modules
	StringLiteral,      // "Renderer", "Meteor", "CoreKit"
	Colon,              // :
	Comma,              // ,
	OpenBrace,          // {
	CloseBrace,         // }
	EndOfFile,			// \0
	Unknown
};

struct ScriptParser
{
	static void BeginParse();

	static bool OpenScript(Module* module);

protected:

	static void Expected(const char* ptr);

	static void GetValue();

	static void AdvanceACharacter();

	static void SkipWhitspace();
	
	static bool IsWhitspace();

	static bool IsAlpha(const char input);

	static inline char* buffer = nullptr;

	static inline Module* currentlyReadModule = nullptr;
};

