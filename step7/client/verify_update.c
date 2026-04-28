#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/err.h>

int verify_certificate(const char *cert_file, const char *ca_file) {
    FILE *cert_fp = fopen(cert_file, "r");
    FILE *ca_fp = fopen(ca_file, "r");

    if (!cert_fp || !ca_fp) {
        printf("Failed to open certificate files\n");
        return 0;
    }

    X509 *cert = PEM_read_X509(cert_fp, NULL, NULL, NULL);
    X509 *ca_cert = PEM_read_X509(ca_fp, NULL, NULL, NULL);

    fclose(cert_fp);
    fclose(ca_fp);

    if (!cert || !ca_cert) {
        printf("Failed to parse certificates\n");
        return 0;
    }

    X509_STORE *store = X509_STORE_new();
    X509_STORE_add_cert(store, ca_cert);

    X509_STORE_CTX *ctx = X509_STORE_CTX_new();
    X509_STORE_CTX_init(ctx, store, cert, NULL);

    int result = X509_verify_cert(ctx);

    if (result == 1)
        printf("Certificate verification: PASS\n");
    else
        printf("Certificate verification: FAIL\n");

    X509_free(cert);
    X509_free(ca_cert);
    X509_STORE_free(store);
    X509_STORE_CTX_free(ctx);

    return result == 1;
}

int verify_signature(const char *bin_file, const char *sig_file, const char *cert_file) {
    FILE *cert_fp = fopen(cert_file, "r");
    FILE *bin_fp = fopen(bin_file, "rb");
    FILE *sig_fp = fopen(sig_file, "rb");

    if (!cert_fp || !bin_fp || !sig_fp) {
        printf("File open error\n");
        return 0;
    }

    X509 *cert = PEM_read_X509(cert_fp, NULL, NULL, NULL);
    fclose(cert_fp);

    EVP_PKEY *pubkey = X509_get_pubkey(cert);

    fseek(bin_fp, 0, SEEK_END);
    long bin_len = ftell(bin_fp);
    rewind(bin_fp);

    unsigned char *bin_data = malloc(bin_len);
    fread(bin_data, 1, bin_len, bin_fp);
    fclose(bin_fp);

    fseek(sig_fp, 0, SEEK_END);
    long sig_len = ftell(sig_fp);
    rewind(sig_fp);

    unsigned char *sig = malloc(sig_len);
    fread(sig, 1, sig_len, sig_fp);
    fclose(sig_fp);

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pubkey);

    int result = EVP_DigestVerify(ctx, sig, sig_len, bin_data, bin_len);

    if (result == 1)
        printf("Signature verification: PASS\n");
    else
        printf("Signature verification: FAIL\n");

    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(pubkey);
    X509_free(cert);
    free(bin_data);
    free(sig);

    return result == 1;
}

int verify_checksum(const char *bin_file, const char *checksum_file) {
    FILE *fp = fopen(bin_file, "rb");
    if (!fp) return 0;

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);

    unsigned char *data = malloc(len);
    fread(data, 1, len, fp);
    fclose(fp);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(data, len, hash);

    free(data);

    FILE *chk_fp = fopen(checksum_file, "r");
    if (!chk_fp) return 0;

    char expected[65];
    fscanf(chk_fp, "%64s", expected);
    fclose(chk_fp);

    char actual[65];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(actual + (i * 2), "%02x", hash[i]);
    actual[64] = '\0';

    if (strcmp(actual, expected) == 0) {
        printf("Checksum verification: PASS\n");
        return 1;
    } else {
        printf("Checksum verification: FAIL\n");
        return 0;
    }
}

int main() {
    printf("=== UPDATE VERIFICATION START ===\n");

    int cert_ok = verify_certificate("software_update.crt", "rootCA.crt");
    int sig_ok = verify_signature("software_update.bin", "software_update.sig", "software_update.crt");
    int chk_ok = verify_checksum("software_update.bin", "software_update.checksum");

    if (cert_ok && sig_ok && chk_ok)
        printf("\nRESULT: UPDATE VALID\n");
    else
        printf("\nRESULT: UPDATE INVALID\n");

    return 0;
}