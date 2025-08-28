/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

class IFile;
class Module;
struct ScriptWordBase 
{
	constexpr ScriptWordBase() {};

	static constexpr const char* GetWord() { return ""; };
	Array<String> container;

	virtual ~ScriptWordBase() = default;
};

#define ADD_SCRIPT_WORD(NewWord)							\
	struct ScriptWord_##NewWord : public ScriptWordBase		\
	{														\
		constexpr ScriptWord_##NewWord() : ScriptWordBase() {}; \
		static constexpr const char* GetWord() { return #NewWord; };\
	}


enum class TokenIdentifier
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

class ScriptParser
{
public:
	enum class ParsingType { MainDescriptor, Module };

	void ParseScript(const ParsingType& type);

	void SetBuffer(const char* newBuffer) { buffer = newBuffer; };

	bool OpenScript(const String& modulePath);

	static bool FindMainScript(String& path);

protected:
	void InputToContainer(ScriptWordBase& word);

	bool Expected(const char*& in, const char* ptr);

	bool ExpectedIdentifier(const char*& in, const TokenIdentifier& identifier);

	TokenIdentifier GetIdentifier(const char*& in);

	String TokenIndetifierToString(const TokenIdentifier& identifier) const;

	String GetValue(const char*& in);

	String GetWord(const char*& in);

	void AdvanceACharacter(const char*& in) { if (*in) { in++; } };

	void SkipWhitspace(const char*& in) 
	{
		while (*in == ' ' || *in == '\n' || *in == '\r' || *in == '\t' || *in == '\v')
			in++;
	}
	
	bool IsWhitspace(const char*& in) { return (*in == ' ' || *in == '\n' || *in == '\r' || *in == '\t' || *in == '\v') ? true : false; }

	bool IsAlpha(const char& input) { return (input >= 'a' && input <= 'z' || input >= 'A' && input <= 'Z'); };

	const char* buffer = nullptr;

	IFile* currentlyReadModule = nullptr;

	struct
	{
		String projectName;

		String projectNameDependsFromSolution;

		enum class EProjectType { Project, Solution };

		Array<String> libraries;

		Array<String> includePaths;

		Array<String> executable;
	};
};

