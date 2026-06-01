/*!
    \file    mqtt_ssl_config.c
    \brief   MQTT ssl shell config for GD32VW55x WiFi SDK

    \version 2023-10-27, V1.0.0, firmware for GD32VW55x
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "mqtt.h"
#ifdef LWIP_SSL_MQTT
#include "mqtt_ssl_config.h"
#include "altcp_tls.h"
#include "mqtt_priv.h"

static const char server_root_CA[] =
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIDXzCCAkegAwIBAgILBAAAAAABIVhTCKIwDQYJKoZIhvcNAQELBQAwTDEgMB4G\r\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp\r\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDkwMzE4MTAwMDAwWhcNMjkwMzE4\r\n" \
"MTAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEG\r\n" \
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\r\n" \
"hvcNAQEBBQADggEPADCCAQoCggEBAMwldpB5BngiFvXAg7aEyiie/QV2EcWtiHL8\r\n" \
"RgJDx7KKnQRfJMsuS+FggkbhUqsMgUdwbN1k0ev1LKMPgj0MK66X17YUhhB5uzsT\r\n" \
"gHeMCOFJ0mpiLx9e+pZo34knlTifBtc+ycsmWQ1z3rDI6SYOgxXG71uL0gRgykmm\r\n" \
"KPZpO/bLyCiR5Z2KYVc3rHQU3HTgOu5yLy6c+9C7v/U9AOEGM+iCK65TpjoWc4zd\r\n" \
"QQ4gOsC0p6Hpsk+QLjJg6VfLuQSSaGjlOCZgdbKfd/+RFO+uIEn8rUAVSNECMWEZ\r\n" \
"XriX7613t2Saer9fwRPvm2L7DWzgVGkWqQPabumDk3F2xmmFghcCAwEAAaNCMEAw\r\n" \
"DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFI/wS3+o\r\n" \
"LkUkrk1Q+mOai97i3Ru8MA0GCSqGSIb3DQEBCwUAA4IBAQBLQNvAUKr+yAzv95ZU\r\n" \
"RUm7lgAJQayzE4aGKAczymvmdLm6AC2upArT9fHxD4q/c2dKg8dEe3jgr25sbwMp\r\n" \
"jjM5RcOO5LlXbKr8EpbsU8Yt5CRsuZRj+9xTaGdWPoO4zzUhw8lo/s7awlOqzJCK\r\n" \
"6fBdRoyV3XpYKBovHd7NADdBj+1EbddTKJd+82cEHhXXipa0095MJ6RMG3NzdvQX\r\n" \
"mcIfeg7jLQitChws/zyrVQ4PkX4268NXSb7hLi18YIvDQVETI53O9zJrlAGomecs\r\n" \
"Mx86OyXShkDOOyyGeMlhLxS67ttVb9+E7gUJTb0o2HLO02JQZR7rkpeDMdmztcpH\r\n" \
"WD9f\r\n" \
"-----END CERTIFICATE-----\r\n";

static const char client_CA[] =
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIEmzCCA4OgAwIBAgIDH0WKMA0GCSqGSIb3DQEBCwUAMG4xCzAJBgNVBAYTAkNO\r\n" \
"MSMwIQYDVQQDDBpvbmxpbmUuaW90ZGV2aWNlLmJhaWR1LmNvbTEOMAwGA1UECgwF\r\n" \
"QkFJRFUxDDAKBgNVBAsMA0JDRTEcMBoGCSqGSIb3DQEJARYNaW90QGJhaWR1LmNv\r\n" \
"bTAeFw0yMzEwMjAwNTIwMDFaFw0zMzEwMTcwNTIwMDFaMEMxDjAMBgNVBAoMBUJh\r\n" \
"aWR1MQswCQYDVQQGEwJDTjEWMBQGA1UEAwwNYWZzdW16eS9mZWlnZTEMMAoGA1UE\r\n" \
"CwwDQkNFMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgMlgtHQqtKg5\r\n" \
"CxpumURDIADkJlcBQJWxa6XmfW+NxyDyT/BsGUiq8L/D9ptnJ7p5hyWDqhAXJSa7\r\n" \
"tGA9433RmlC43grSpxEw64EQkQ6LGLxXQ8Qk50W2ySOnR+zvH0VyB69wmUXwLa/L\r\n" \
"/wv5DPzoZhcAZV5AIPJJt3WwDwZHLMtJyY+XURFGGIuMYTs+CA5yvxEZeCWFi9Y7\r\n" \
"P87YZrgX6xlSfBp962pTNeiwdVgvvOPrNWMIjCkpxPSxbYtcJYrwaAmit1dUoNhP\r\n" \
"bS0Udyfw3dhoK9MzIabiqdECTZnsWRHyKCSJAx3sgJphgqAz9UHVYDfjRJjWswNm\r\n" \
"0dGKAwD3TwIDAQABo4IBazCCAWcwHQYDVR0OBBYEFDWqaWRg6M5WUHmPTZzZyLN4\r\n" \
"bnI7MAwGA1UdEwEB/wQCMAAwHwYDVR0jBBgwFoAUs+sTxDBPAMKn6xUOzNgrJ3YT\r\n" \
"ZFcwgaMGA1UdHwSBmzCBmDCBlaCBkqCBj4aBjGh0dHA6Ly9wa2lpb3YuYmFpZHVi\r\n" \
"Y2UuY29tL3YxL3BraS9jcmw/Y21kPWNybCZmb3JtYXQ9UEVNJmlzc3Vlcj1DPUNO\r\n" \
"LENOPW9ubGluZS5pb3RkZXZpY2UuYmFpZHUuY29tLEVNQUlMQUREUkVTUz1pb3RA\r\n" \
"YmFpZHUuY29tLE89QkFJRFUsT1U9QkNFMEIGCCsGAQUFBwEBBDYwNDAyBggrBgEF\r\n" \
"BQcwAYYmaHR0cDovL3BraWlvdi5iYWlkdWJjZS5jb20vdjEvcGtpL29jc3AwDgYD\r\n" \
"VR0PAQH/BAQDAgP4MB0GA1UdJQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDBDANBgkq\r\n" \
"hkiG9w0BAQsFAAOCAQEAg4pvwaMa89SspoQbfFPZxIw1N94qxXNuF/wgCMHACmRj\r\n" \
"T4omZezVXmfxG8aSPDMPnUYpaIYqdyhWijid3jNG2c5K2RXGfvxkF6sFeb+0rOd4\r\n" \
"4XYfdF1SyZil5bIVxmWKsikb9zM4VW/hQ76b8cOZNqmBWbx+YlphwQ/FQU7tD4Hl\r\n" \
"qD2AcikbRFYv153/0c8gjFF2HKhsAZhL7LoHrB8MQkBNGy4679TiYy4id/JEIA8j\r\n" \
"N3JWqLgOO7txDQZCIO5axvbSxIUWkgv4TVhU1dxRzzFswpJHWIwICoadaJh9ceCX\r\n" \
"iYZLprcqj/WE5BRR7PvtSngBZn2VBxhth0yE4CX4aA==\r\n" \
"-----END CERTIFICATE-----\r\n";

static const char client_private_key[] =
"-----BEGIN RSA PRIVATE KEY-----\r\n" \
"MIIEowIBAAKCAQEAgMlgtHQqtKg5CxpumURDIADkJlcBQJWxa6XmfW+NxyDyT/Bs\r\n" \
"GUiq8L/D9ptnJ7p5hyWDqhAXJSa7tGA9433RmlC43grSpxEw64EQkQ6LGLxXQ8Qk\r\n" \
"50W2ySOnR+zvH0VyB69wmUXwLa/L/wv5DPzoZhcAZV5AIPJJt3WwDwZHLMtJyY+X\r\n" \
"URFGGIuMYTs+CA5yvxEZeCWFi9Y7P87YZrgX6xlSfBp962pTNeiwdVgvvOPrNWMI\r\n" \
"jCkpxPSxbYtcJYrwaAmit1dUoNhPbS0Udyfw3dhoK9MzIabiqdECTZnsWRHyKCSJ\r\n" \
"Ax3sgJphgqAz9UHVYDfjRJjWswNm0dGKAwD3TwIDAQABAoIBACAD78psZoFqoGOm\r\n" \
"OoBXe+hk0FLHpkQL8oM0RsxAAxrRketVdUoDypr20RxpHYe+z59NMCICxf4yVs9M\r\n" \
"bZY0HPsjvhFU13E40NR6zUeCOgn4KClshVAJAJuYBWnX+MnpVaObdX5k6IQzA59v\r\n" \
"toYICS334d1RRNfr4298Djxt7xuvQul4g0MVoDGnnTDyfu06WtYYonfc1qL+MZYw\r\n" \
"KV3s+Cq9X2k2TypN5+VrrG0XEqDVV84UDzw9TfimBEF699Z5YW2osOFyBX+rtrds\r\n" \
"HKcQRXL1FHWtPNOCHYQMzbyByqwAjQOJSzKomMpbVVW130DUMZaRPriiapqkuPpr\r\n" \
"m7nM/QECgYEAyg2G2tRLSWUsNKJ9rQjKkhcEm5RTQP7iLciDQAF/AH1F5/NCg/IW\r\n" \
"UL+pZ7LnaEfflAkPx1r/Yj3VIslSNWWGgbo2LC5hZZFiipSPiA0Yr9DvZSnx5M0h\r\n" \
"vsDtW9TheKCmAk5ObQhtnvJJGMMuPun6oHDt+Cm1MQ+ZkeLyaBoHgmECgYEAoywM\r\n" \
"DlQH3kqJaNUO5msp2v4aT38kUH7MYh92V5/D5pjLBd3LNh1TeOMtl/E2BByVVQuU\r\n" \
"ug20pk+PvEfXdQrLDJSmzCuD/4BTKYzVTEEguBsjWTLAxYzqcW+sH4F26MmHo5vj\r\n" \
"1MSDPv2QtScaIvJ1vimCRvdsHNqLxz6fbcBcN68CgYBYqXLhl4Kp5EFvn1Xylgb9\r\n" \
"8CfPdVjLDo2FdZVSgtWOC2qfi7lGWPa93DykCndM5S0QsqE/44hpPaTHLPxr/e72\r\n" \
"AhY/cOLARPmuwd3x331TuSUziSJiOjlykQoW3+VIn4X5QQ9c/PPNaZf1y8ABT37w\r\n" \
"5F0oJnUh4CyNPb8NO07MQQKBgHHUbYilWGPbjaZjU6Ssx6MtNv+US6oX+s7M8grI\r\n" \
"uqoolyE9i+DxbmTb3terfyo5IngUvylYHFkVEcmgOI8++02Ieh/ej5PzWpCW3cn3\r\n" \
"eTLWQ7+bJ13pIzgFVocYEvLsfEJHoxWwDXso+wVVBOeySy9g17BYNMSgNXjGmAPB\r\n" \
"eNOjAoGBAI0w1Zq/YXL/fXnvIpzPfUzewCOtEUk0DwjsKly96pEm5HM91zwH5w8L\r\n" \
"VFesuvRAkrXs+OLizwwfAAwRt2uf3Fq4tIuJeLDx2x9CxqPJQEkXmwWkSptFVyg6\r\n" \
"kJZPMWZeH1iWOlm5aVI5GwRoLaqIw/2KOhprfVRsU6uiedmWecp/\r\n" \
"-----END RSA PRIVATE KEY-----\r\n";

int mqtt_ssl_CA_cfg(struct mqtt_client_s *client, u8_t mutual_auth)
{
    if (mutual_auth) {
        client->tls_config = altcp_tls_create_config_client_2wayauth((u8_t *)server_root_CA, sizeof(server_root_CA),
                                        (u8_t *)client_private_key, sizeof(client_private_key), NULL, 0,
                                        (u8_t *)client_CA, sizeof(client_CA));
    } else {
        client->tls_config = altcp_tls_create_config_client((u8_t *)server_root_CA, sizeof(server_root_CA));
    }

    LWIP_ERROR("tls config failed!", (client->tls_config != NULL), return ERR_MEM);

    return 0;
}

void mqtt_ssl_CA_free(struct mqtt_client_s *client)
{
    altcp_tls_free_config(client->tls_config);
    return;
}

#endif/*LWIP_SSL_MQTT*/

