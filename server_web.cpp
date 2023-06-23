#include "defines.h"

WebServer server(80);

void handleRoot() {
        String html = R"HTML(
                <!DOCTYPE html>
                <html>
                <head>
                <title>Kitt's Kitchen Kullanıcı Arayüzü</title>
                <style>
                body {
                background-color: #f4f4f4;
                font-family: Arial, sans-serif;
                }

                h2 {
                color: #333;
                text-align: center;
                padding-top: 20px;
                }

                form {
                max-width: 400px;
                margin: 0 auto;
                padding: 20px;
                background-color: #fff;
                border-radius: 5px;
                box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
                }

                label {
                display: block;
                font-weight: bold;
                margin-bottom: 10px;
                color: #666;
                }

            input[type="number"],
            input[type="text"] {
                width: 100%;
                padding: 10px;
                border: 1px solid #ccc;
                border-radius: 4px;
                box-sizing: border-box;
                margin-bottom: 10px;
            }

            input[type="button"] {
                background-color: #ff8000;
                color: #fff;
                border: none;
                padding: 10px 20px;
                font-size: 16px;
                border-radius: 4px;
                cursor: pointer;
            }

            input[type="button"]:hover {
                background-color: #ff8000;
            }

            input[type="checkbox"] {
                margin-bottom: 10px;
            }
        </style>
    </head>
    <body>
        <h2>Kitt's Kitchen</h2>
        <form>
            <label for="saat1">Aksam Mama Saati:</label>
            <label for="saat1">Saat:</label>
            <input type="number" id="saatAksam" name="saatAksam1" min="12" max="23" required>
            <label for="dakika1">Dakika:</label>
            <input type="number" id="dakika1" name="dakika1" min="0" max="59" required>
            <input type="checkbox" id="dakikaCheck1" name="dakikaCheck1"><br><br>

            <label for="saat2">Sabah Mama Saati:</label>
            <label for="saat2">Saat:</label>
            <input type="number" id="saatSabah" name="saatSabah1" min="0" max="12" required>
            <label for="dakika2">Dakika:</label>
            <input type="number" id="dakika2" name="dakika2" min="0" max="59" required>
            <input type="checkbox" id="dakikaCheck2" name="dakikaCheck2"><br><br>

            <label for="miktar">Miktar:</label>
            <input type="text" id="miktar" name="miktar" pattern="[0-9]+" required><br><br>

            <input type="button" value="Kaydet" onclick="kaydet()">
            <input type="button" value="Mama Dok" onclick="dok()">
        </form>
        <script>
            function kaydet() {
                var saat1 = parseInt(document.getElementById("saatAksam").value);
                var dakika1 = parseInt(document.getElementById("dakika1").value);
                var saat2 = parseInt(document.getElementById("saatSabah").value);
                var dakika2 = parseInt(document.getElementById("dakika2").value);
                var miktar = parseInt(document.getElementById("miktar").value);
                var aksamKontrol = Boolean(document.getElementById("dakikaCheck1").value);
                var sabahKontrol = Boolean(document.getElementById("dakikaCheck2").value);

                var xhr = new XMLHttpRequest();
                xhr.open("POST", "/kaydet", true);
                xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
                xhr.send("saat1=" + saat1 + "&dakika1=" + dakika1 + "&saat2=" + saat2 + "&dakika2=" + dakika2 + "&miktar=" + miktar + "&aksamKontrol=" + aksamKontrol + "&sabahKontrol=" + sabahKontrol);
            }

            function dok() {
                var mamaDok = Boolean("true");
                // Burada mama dökme işlemini gerçekleştirebilirsiniz
            }
        </script>
    </body>
    </html>
  )HTML";
  server.send(200, "text/html", html);
}

String get_web_hour_night()
{
    return (server.arg("saat1"));
}

String get_web_minute_night()
{
    return (server.arg("dakika1"));
}

String get_web_hour_morning()
{
    return (server.arg("saat2"));
}

String get_web_minute_morning()
{
    return (server.arg("dakika2"));
}

String get_web_food_value()
{
    return (server.arg("miktar"));
}

int isMornign()
{
    return (server.arg("sabahKontrol") ? 1 : 0);
}

int isNight()
{
    return (server.arg("aksamKontrol") ? 1 : 0);
}

int foodCheck()
{
  return (server.arg("mamaDok") ? 0 : 1);
}


void handleKaydet() {
    // Serial.print("Akşam Mama Saati: ");
    // Serial.print(saat1);
    // Serial.print(":");
    // Serial.print(dakika1);
    // Serial.print(" - Sabah Mama Saati: ");
    // Serial.print(saat2);
    // Serial.print(dakika2);
    // Serial.print(" - Miktar: ");
    // Serial.print(miktar);
    // Serial.print(" - Akşam Kontrol: ");
    // Serial.print(aksamKontrol);
    // Serial.print(" - Sabah Kontrol: ");
    // Serial.println(sabahKontrol);
    server.send(200, "text/plain", "Değerler kaydedildi");
}


void set_web_server()
{
    server.on("/", handleRoot);
    server.on("/kaydet", handleKaydet);
    server.begin();
    Serial.println(WiFi.localIP());
}

void get_web_server()
{
    server.handleClient();
}
