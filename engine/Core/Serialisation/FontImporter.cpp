/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "FontImporter.h"
//#include <FreeType/ft2build.h>
//#include FT_FREETYPE_H

//FT_Library library;
//FT_Face face;

FontImporter::FontImporter(const String Name)
{
	loadAsset(Name);
}

void FontImporter::loadAsset(const String Name)
{
	//FT_Error code = FT_Init_FreeType(&library);
	//if (code != 0)
	//{

	//}

	//code = FT_New_Face(library, "E:\\meteor\\product\\DINPro Bold.otf", 0, &face);
	//if (code == FT_Err_Unknown_File_Format)
	//{

	//}

	//FT_Set_Pixel_Sizes(face, 0, 48);

	//code = FT_Load_Char(face, 'T', FT_LOAD_Render);
	//if (code != 0)
	//	return;

	//FT_Bitmap& bm = face->glyph->bitmap;

}
