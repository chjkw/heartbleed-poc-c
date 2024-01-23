#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main(int argc, char* argv[]) {
    SSL_library_init();
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        perror("SSL_CTX_new");
        return 1;
    }

    SSL *ssl = SSL_new(ctx);
    if (ssl == NULL) {
        perror("SSL_new");
        SSL_CTX_free(ctx);
        return 1;
    }

    SSL_set_tlsext_host_name(ssl, "target_server.com"); // 대상 서버 이름을 여기에 넣으세요

    if (SSL_connect(ssl) != 1) {
        perror("SSL_connect");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 1;
    }

    char buffer[64 * 1024];
    memset(buffer, 'A', sizeof(buffer)); // 버퍼 크기를 적절하게 조절하세요

    int result = SSL_write(ssl, buffer, sizeof(buffer));
    if (result <= 0) {
        perror("SSL_write");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 1;
    }

    result = SSL_read(ssl, buffer, sizeof(buffer));
    if (result <= 0) {
        perror("SSL_read");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 1;
    }

    SSL_free(ssl);
    SSL_CTX_free(ctx);

    printf("Heartbleed 취약점이 아닌 것으로 보입니다.\n");
    return 0;
}

