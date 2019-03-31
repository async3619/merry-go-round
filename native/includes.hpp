#ifndef MERRY_GO_ROUND_INCLUDES_HPP
#define MERRY_GO_ROUND_INCLUDES_HPP

#define WIN32_LEAN_AND_MEAN
#define TAGLIB_STATIC
#define BOOST_LIB_TOOLSET "vc140"

#include <iostream>
#include <unordered_map>
#include <type_traits>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/asffile.h>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/flacfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/mpcfile.h>
#include <taglib/mp4file.h>
#include <taglib/wavpackfile.h>
#include <taglib/speexfile.h>
#include <taglib/opusfile.h>
#include <taglib/trueaudiofile.h>
#include <taglib/aifffile.h>
#include <taglib/wavfile.h>
#include <taglib/apefile.h>
#include <taglib/modfile.h>
#include <taglib/s3mfile.h>
#include <taglib/itfile.h>
#include <taglib/xmfile.h>
#include <taglib/dsffile.h>
#include <taglib/dsdifffile.h>

#include <boost/type_index.hpp>
#include <boost/locale.hpp>

#include "utils.hpp"

#include "releasable.hpp"
#include "dictionary.hpp"
#include "encoder.hpp"
#include "music.hpp"

#include <nbind/nbind.h>

#endif // MERRY_GO_ROUND_INCLUDES_HPP
