#include "FontAtlasBuilder.h"


#include <thread>

#include <Library/Externals/DirectXTex/DirectXTex.h>

#include <Library/Externals/msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

using namespace msdf_atlas;

void FontAtlasBuilder::entry_point(const std::filesystem::path& path) {
	// ロード
	msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
	if (ft) {
		msdfgen::FontHandle* font = loadFont(ft, "C:\\Windows\\Fonts\\arialbd.ttf");
		if (font) {
			HRESULT hr;
			// Storage for glyph geometry and their coordinates in the atlas
			std::vector<GlyphGeometry> glyphs;
			// FontGeometry is a helper class that loads a set of glyphs from a single font.
			// It can also be used to get additional font metrics, kerning information, etc.
			FontGeometry fontGeometry(&glyphs);
			// Load a set of character glyphs:
			// The second argument can be ignored unless you mix different font sizes in one atlas.
			// In the last argument, you can specify a charset other than ASCII.
			// To load specific glyph indices, use loadGlyphs instead.
			fontGeometry.loadCharset(font, 1.0, Charset::ASCII);
			// Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
			const double maxCornerAngle = 3.0;
			for (GlyphGeometry& glyph : glyphs)
				glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
			// TightAtlasPacker class computes the layout of the atlas.
			TightAtlasPacker packer;
			// Set atlas parameters:
			// setDimensions or setDimensionsConstraint to find the best value
			packer.setDimensionsConstraint(DimensionsConstraint::SQUARE);
			// setScale for a fixed size or setMinimumScale to use the largest that fits
			packer.setMinimumScale(24.0);
			// setPixelRange or setUnitRange
			packer.setPixelRange(2.0);
			packer.setMiterLimit(1.0);
			// Compute atlas layout - pack glyphs
			packer.pack(glyphs.data(), static_cast<int>(glyphs.size()));
			// Get final atlas dimensions
			int width = 0, height = 0;
			packer.getDimensions(width, height);
			// The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
			ImmediateAtlasGenerator<
				float, // pixel type of buffer for individual glyphs depends on generator function
				3, // number of atlas color channels
				msdfGenerator, // function to generate bitmaps for individual glyphs
				BitmapAtlasStorage<byte, 3> // class that stores the atlas bitmap
				// For example, a custom atlas storage class that stores it in VRAM can be used.
			> generator(width, height);
			// GeneratorAttributes can be modified to change the generator's default settings.
			GeneratorAttributes attributes;
			generator.setAttributes(attributes);
			generator.setThreadCount(std::thread::hardware_concurrency());
			// Generate atlas bitmap
			generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));

			msdfgen::BitmapConstRef<byte, 3> bitmapRef = generator.atlasStorage();

			DirectX::Image image = {};
			image.width = bitmapRef.width;
			image.height = bitmapRef.height;
			image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch = sizeof(byte) * 3 * bitmapRef.width;
			image.slicePitch = image.rowPitch * bitmapRef.height;
			image.pixels = const_cast<uint8_t*>(bitmapRef.pixels);

			DirectX::ScratchImage scratchImage;
			hr = scratchImage.InitializeFromImage(image);
			assert(SUCCEEDED(hr));

			// 圧縮
			DirectX::ScratchImage compressed;
			hr = DirectX::Compress(scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(),
				DXGI_FORMAT_BC7_UNORM, DirectX::TEX_COMPRESS_PARALLEL, 1.0f, compressed);
			assert(SUCCEEDED(hr));

			// 転送
			scratchImage = std::move(compressed);
			DirectX::TexMetadata metadata = scratchImage.GetMetadata();

			std::filesystem::path outputPath = path.parent_path() / (path.stem().native() + L".dds");
			hr = DirectX::SaveToDDSFile(scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, DirectX::DDS_FLAGS_NONE, outputPath.c_str());
			assert(SUCCEEDED(hr));

			destroyFont(font);
		}
		deinitializeFreetype(ft);
	}
}
