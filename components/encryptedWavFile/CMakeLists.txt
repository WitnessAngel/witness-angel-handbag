idf_component_register(SRCS "encryptedWavFile.c"
                       INCLUDE_DIRS "include"
				       PRIV_REQUIRES mbedtls encrypter
                       )
