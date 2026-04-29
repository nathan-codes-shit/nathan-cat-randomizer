# Nathan's Cat Randomizer

A small cosmetic Geode mod for Geometry Dash.

During gameplay, the mod places a semi-transparent cat in the bottom-left corner. Every click picks another random bundled cat.

## Features

- 330 bundled cat images
- Bottom-left gameplay overlay
- Random cat swap on click
- Settings for enabling the overlay, opacity, and cat size
- No gameplay changes

## Assets

Bundled cat sources are listed in `CATALOG.md`.

The current bundle contains CATAAS images plus one public-domain Wikimedia Commons image. The previous attribution-required Wikimedia image and entries without retained source URLs were removed before release.

## Building

Requires the Geode SDK and Geode CLI.

```powershell
cmake -S . -B build
cmake --build build --config Release
```

The packaged mod will be generated in `build/`.

