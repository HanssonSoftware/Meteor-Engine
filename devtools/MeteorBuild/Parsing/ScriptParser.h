/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

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
	static inline char* position = nullptr;

	static inline Module* currentlyReadModule = nullptr;

	static void AdvanceACharacter();

	static void SkipWhitspace();
};

