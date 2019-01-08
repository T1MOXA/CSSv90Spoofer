#pragma once

static void CreateRandomString(char *pszDest, int nLength)
{
	static const char c_szAlphaNum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i < nLength; ++i)
		pszDest[i] = c_szAlphaNum[rand() % (sizeof(c_szAlphaNum) - 1)];

	pszDest[nLength] = '\0';
}

const int SIZE_REVEMU2013 = 168 + 8; // 8 for steamid

static int GenerateRevEmu2013(void *pDest, int nSteamID = 0, int *outRevHash = nullptr)
{
	char szhwid[33];

	for (int tries = 5; ; tries--)
	{
		CreateRandomString(szhwid, 32);
		if (!RevSpoof(szhwid, nSteamID) && !tries)
			return 0;
		else
			break;
	}

	memset(pDest, 0, SIZE_REVEMU2013);

	auto pTicket = (int *)pDest;
	auto pbTicket = (unsigned char *)pDest;

	auto revHash = RevHash(szhwid);

	if (revHash == 0)
		revHash = 0xDFDC1C3D;

	pTicket[0] = 'S';                      // +0
	pTicket[1] = revHash;                  // +4
	pTicket[2] = 'rev';                    // +8
	pTicket[3] = 0;                        // +12
	pTicket[4] = revHash << 1;             // +16
	pTicket[5] = 0x01100001;               // +20
	pTicket[6] = (int)_time64(0) + 90123;  // +24
	pbTicket[27] = ~(pbTicket[27] + pbTicket[24]);
	pTicket[7] = ~(int)_time64(0);         // +28
	pTicket[8] = revHash * 2 >> 3;         // +32
	pTicket[9] = 0;                        // +36

	if (outRevHash)
		*outRevHash = revHash;

	static const char c_szAESKeyRand[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

	char szAESHashRand[32];
	auto AESRand = CRijndael();
	AESRand.MakeKey(c_szAESKeyRand, CRijndael::sm_chain0, 32, 32);
	AESRand.EncryptBlock(szhwid, szAESHashRand);
	memcpy(&pbTicket[40], szAESHashRand, 32);

	static const char c_szAESKeyRev[] = "_YOU_SERIOUSLY_NEED_TO_GET_LAID_";
	char AESHashRev[32];
	auto AESRev = CRijndael();
	AESRev.MakeKey(c_szAESKeyRev, CRijndael::sm_chain0, 32, 32);
	AESRev.EncryptBlock(c_szAESKeyRand, AESHashRev);
	memcpy(&pbTicket[72], AESHashRev, 32);

	char szSHAHash[32];
	auto sha = CSHA(CSHA::SHA256);
	sha.AddData(szhwid, 32);
	sha.FinalDigest(szSHAHash);
	memcpy(&pbTicket[104], szSHAHash, 32);

	static auto pzsSentimentMsg = "I hate birzyk and his emulators";
	memcpy((char *)&pbTicket[136], pzsSentimentMsg, strlen(pzsSentimentMsg) + 1);

	return SIZE_REVEMU2013;
}