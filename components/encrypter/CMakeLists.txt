idf_component_register(SRCS "encrypter.c" "utils.c" "generation.c"
                       INCLUDE_DIRS "include"
					   REQUIRES mbedtls
				       PRIV_REQUIRES padding bootloader_support
				      )
