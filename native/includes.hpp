#ifndef MERRY_GO_ROUND_INCLUDES_HPP
#define MERRY_GO_ROUND_INCLUDES_HPP

#define TAGLIB_STATIC

#include "preprocessors.hpp"

#include <iostream>
#include <cstdio>
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
#include <taglib/chapterframe.h>
#include <taglib/commentsframe.h>
#include <taglib/eventtimingcodesframe.h>
#include <taglib/ownershipframe.h>
#include <taglib/popularimeterframe.h>
#include <taglib/relativevolumeframe.h>

#include <brigand/sequences/list.hpp>
#include <brigand/sequences/map.hpp>
#include <brigand/sequences/at.hpp>
#include <brigand/algorithms/for_each.hpp>
#include <brigand/adapted/pair.hpp>

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
#include "node/array.hpp"

#include "music/internal.hpp"
#include "music/internal/mpeg.hpp"
#include "music/music.hpp"

#include "io/file.hpp"

#endif // MERRY_GO_ROUND_INCLUDES_HPP
