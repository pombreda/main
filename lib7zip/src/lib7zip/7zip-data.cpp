#include <lib7zip/7zip.hpp>

namespace SevenZip {
	const Base::NamedValues<int> ArcItemPropsNames[63] = {
		{L"kpidNoProperty      ", kpidNoProperty},
		{L"kpidMainSubfile     ", kpidMainSubfile},
		{L"kpidHandlerItemIndex", kpidHandlerItemIndex},
		{L"kpidPath            ", kpidPath},
		{L"kpidName            ", kpidName},
		{L"kpidExtension       ", kpidExtension},
		{L"kpidIsDir           ", kpidIsDir},
		{L"kpidSize            ", kpidSize},
		{L"kpidPackSize        ", kpidPackSize},
		{L"kpidAttrib          ", kpidAttrib},
		{L"kpidCTime           ", kpidCTime},
		{L"kpidATime           ", kpidATime},
		{L"kpidMTime           ", kpidMTime},
		{L"kpidSolid           ", kpidSolid},
		{L"kpidCommented       ", kpidCommented},
		{L"kpidEncrypted       ", kpidEncrypted},
		{L"kpidSplitBefore     ", kpidSplitBefore},
		{L"kpidSplitAfter      ", kpidSplitAfter},
		{L"kpidDictionarySize  ", kpidDictionarySize},
		{L"kpidCRC             ", kpidCRC},
		{L"kpidType            ", kpidType},
		{L"kpidIsAnti          ", kpidIsAnti},
		{L"kpidMethod          ", kpidMethod},
		{L"kpidHostOS          ", kpidHostOS},
		{L"kpidFileSystem      ", kpidFileSystem},
		{L"kpidUser            ", kpidUser},
		{L"kpidGroup           ", kpidGroup},
		{L"kpidBlock           ", kpidBlock},
		{L"kpidComment         ", kpidComment},
		{L"kpidPosition        ", kpidPosition},
		{L"kpidPrefix          ", kpidPrefix},
		{L"kpidNumSubDirs      ", kpidNumSubDirs},
		{L"kpidNumSubFiles     ", kpidNumSubFiles},
		{L"kpidUnpackVer       ", kpidUnpackVer},
		{L"kpidVolume          ", kpidVolume},
		{L"kpidIsVolume        ", kpidIsVolume},
		{L"kpidOffset          ", kpidOffset},
		{L"kpidLinks           ", kpidLinks},
		{L"kpidNumBlocks       ", kpidNumBlocks},
		{L"kpidNumVolumes      ", kpidNumVolumes},
		{L"kpidTimeType        ", kpidTimeType},
		{L"kpidBit64           ", kpidBit64},
		{L"kpidBigEndian       ", kpidBigEndian},
		{L"kpidCpu             ", kpidCpu},
		{L"kpidPhySize         ", kpidPhySize},
		{L"kpidHeadersSize     ", kpidHeadersSize},
		{L"kpidChecksum        ", kpidChecksum},
		{L"kpidCharacts        ", kpidCharacts},
		{L"kpidVa              ", kpidVa},
		{L"kpidId              ", kpidId},
		{L"kpidShortName       ", kpidShortName},
		{L"kpidCreatorApp      ", kpidCreatorApp},
		{L"kpidSectorSize      ", kpidSectorSize},
		{L"kpidPosixAttrib     ", kpidPosixAttrib},
		{L"kpidLink            ", kpidLink},
		{L"kpidError           ", kpidError},
		{L"kpidTotalSize       ", kpidTotalSize},
		{L"kpidFreeSpace       ", kpidFreeSpace},
		{L"kpidClusterSize     ", kpidClusterSize},
		{L"kpidVolumeName      ", kpidVolumeName},
		{L"kpidLocalName       ", kpidLocalName},
		{L"kpidProvider        ", kpidProvider},
		{L"kpidUserDefined     ", kpidUserDefined},
	};

	const Base::NamedValues<CompressMethod> CompressMethodsNames[21] = {
		{L"Copy", metCopy},
		{L"Delta", metDelta},
		{L"7zAES", met7zAES},
		{L"ARM", metARM},
		{L"ARMT", metARMT},
		{L"BCJ", metBCJ},
		{L"BCJ2", metBCJ2},
		{L"BZip2", metBZip2},
		{L"Deflate", metDeflate},
		{L"Deflate64", metDeflate64},
		{L"IA64", metIA64},
		{L"LZMA", metLZMA},
		{L"LZMA2", metLZMA2},
		{L"PPC", metPPC},
		{L"PPMD", metPPMD},
		{L"Rar1", metRar1},
		{L"Rar2", metRar2},
		{L"Rar3", metRar3},
		{L"SPARC", metSPARC},
		{L"Swap2", metSwap2},
		{L"Swap4", metSwap4},
	};
}
