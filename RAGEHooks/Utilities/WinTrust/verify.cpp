#include "../../pch.h"
#include "verify.h"
__forceinline BOOL IsSelfSignedCertificate(PCCERT_CONTEXT pCertContext) {
    if (pCertContext == NULL)
    {
        return FALSE;
    }
    return CertCompareCertificateName(
        pCertContext->dwCertEncodingType,
        &pCertContext->pCertInfo->Subject,
        &pCertContext->pCertInfo->Issuer);
}

BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile) {
    VMProtectBeginMutation("VerifyEmbeddedSignature");
    LONG lStatus;
    DWORD dwLastError;

    WINTRUST_FILE_INFO FileData;
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath = pwszSourceFile;
    FileData.hFile = NULL;
    FileData.pgKnownSubject = NULL;

    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA WinTrustData;

    memset(&WinTrustData, 0, sizeof(WinTrustData));
    WinTrustData.cbStruct = sizeof(WinTrustData);
    WinTrustData.pPolicyCallbackData = NULL;
    WinTrustData.pSIPClientData = NULL;
    WinTrustData.dwUIChoice = WTD_UI_NONE;
    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;
    WinTrustData.dwStateAction = WTD_STATEACTION_VERIFY;
    WinTrustData.hWVTStateData = NULL;
    WinTrustData.pwszURLReference = NULL;
    WinTrustData.dwUIContext = 0;
    WinTrustData.pFile = &FileData;

    lStatus = WinVerifyTrust(NULL, &WVTPolicyGUID, &WinTrustData);

    switch (lStatus) {
    case ERROR_SUCCESS: {
        HCERTSTORE hStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, NULL, CERT_SYSTEM_STORE_CURRENT_USER | CERT_STORE_OPEN_EXISTING_FLAG, L"TrustedPublisher");
        if (hStore) {
            PCCERT_CONTEXT pCertContext = NULL;
            while ((pCertContext = CertEnumCertificatesInStore(hStore, pCertContext))) {
                if (!IsSelfSignedCertificate(pCertContext)) {
                    CertFreeCertificateContext(pCertContext);
                    CertCloseStore(hStore, 0);
                    return TRUE; 
                }
            }
            CertCloseStore(hStore, 0);
        }
        return FALSE;
    }
    case TRUST_E_NOSIGNATURE:
    case TRUST_E_EXPLICIT_DISTRUST:
    case TRUST_E_SUBJECT_NOT_TRUSTED:
    case CRYPT_E_SECURITY_SETTINGS:
        return FALSE;
    default:
        return FALSE;
    }
    return FALSE;
    VMProtectEnd();
}
