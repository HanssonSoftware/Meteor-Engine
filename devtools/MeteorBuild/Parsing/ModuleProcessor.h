/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>
#include <Platform/File.h>

class Module;
struct ScriptWordBase 
{
	ScriptWordBase() { Register(this); };

	virtual const char* GetWord() const = 0;

	virtual ~ScriptWordBase() = default;

	virtual void Invoke(const String& args) = 0;

	void operator()(const String& args) { Invoke(args); };

	static Array<ScriptWordBase*>& GetRegistry() 
	{
		static Array<ScriptWordBase*> instance;
		return instance;
	}

	static ScriptWordBase* Find(const String& word)
	{
		Array<ScriptWordBase*>& reg = GetRegistry();
		for (uint32_t i = 0; i < reg.GetSize(); i++)
		{
			if (strcmp(word.Chr(), reg[i]->GetWord()) == 0)
				return reg[i];
		}

		return nullptr;
	}
private:
	static void Register(ScriptWordBase* word) { GetRegistry().Add(word); }
};

#define ADD_SCRIPT_WORD(Name, Body)                             \
    struct ScriptWord_##Name : public ScriptWordBase            \
    {                                                           \
        const char* GetWord() const override { return #Name; }  \
		virtual void Invoke(const String& args)					\
		{														\
			Body(args);											\
		};														\
    };                                                          \
    static ScriptWord_##Name Global_WordInstance_##Name


enum TokenIdentifier
{
	Identifier			= 1 << 0,         // Project, IncludePath, Modules
	StringLiteral		= 1 << 1,		  // "Renderer", "Meteor", "CoreKit"
	Colon				= 1 << 2,		  // :
	Comma				= 1 << 3,         // ,
	OpenBrace			= 1 << 4,         // {
	CloseBrace			= 1 << 5,         // }
	EndOfFile			= 1 << 6,		  // \0
	UnknownIdentifier	= 0 << 7,
};

class ModuleProcessor
{
public:
	enum class ParsingType { MainDescriptor, Module };

	void ParseScript(const char* buffer);

	void ParseSolutionDescriptor(const char* buffer);

	bool OpenScript(const String& modulePath);

	const char* GetBuffer() { return currentlyReadModule ? currentlyReadModule->GetBuffer() : ""; }
protected:
	void InputToContainer(ScriptWordBase& word);

	bool Expected(const char*& in, const char* ptr);

	bool ExpectedIdentifier(const char*& in, const int& identifier, bool bStep);

	int GetIdentifier(const char*& in);

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

	// const char* buffer = nullptr; buffer is already in the class below!

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

