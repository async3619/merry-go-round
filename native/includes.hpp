#ifndef MERRY_GO_ROUND_INCLUDES_HPP
#define MERRY_GO_ROUND_INCLUDES_HPP

#define WIN32_LEAN_AND_MEAN
#define TAGLIB_STATIC

#include <iostream>
#include <cstring>
#include <functional>
#include <unordered_map>
#include <vector>
#include <type_traits>

#include <taglib/fileref.h>
#include <taglib/tfilestream.h>
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
#include <taglib/privateframe.h>
#include <taglib/tbytevectorstream.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/type_index.hpp>
#include <boost/locale.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/at.hpp>
#include <boost/type.hpp>
#include <boost/filesystem.hpp>

#include <napi.h>

#include "releasable.hpp"

#include "utils/utils.hpp"
#include "debug/debug.hpp"

#include "utils/encoder.hpp"
#include "utils/object-collector.hpp"

#include "node/base.hpp"
#include "node/string.hpp"
#include "node/object.hpp"
#include "node/buffer.hpp"

#include "music/internal.hpp"
#include "music/internal/mpeg.hpp"
#include "music/music.hpp"

#endif // MERRY_GO_ROUND_INCLUDES_HPP
