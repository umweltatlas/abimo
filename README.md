# Wasserhaushaltsmodell Berlin ABIMO 3.2

## Einleitung
Das Wasserhaushaltsmodell Berlin ABIMO 3.2 wurde ursprünglich von der Bundesanstalt für Gewässerkunde entwickelt (Glugla & König, 1989) und für Berliner Verhältnisse angepasst.

Die Berechnungsergebnisse sind für verschiedene Jahre im Umweltatlas Berlin veröffentlicht:

Wasserhaushalt: https://www.berlin.de/umweltatlas/wasser/wasserhaushalt/ 

Grundwasserneubildung: https://www.berlin.de/umweltatlas/wasser/grundwasserneubildung/ 

## Historie
Im Jahre 1993 begann eine gemeinsame Kooperation zwischen Herrn Dr. Glugla von der Bundesanstalt für Gewässerkunde (BfG Außenstelle Berlin, ehemals Institut für Wasserwirtschaft der DDR) und seiner Arbeitsgruppe auf der einen und der Arbeitsgruppe Umweltatlas der Senatsverwaltung für Umweltschutz auf der anderen Seite. Dabei konnten beide Seiten profitieren. Bei der Bundesanstalt für Gewässerkunde wurde zu Beginn der 90er Jahre das Modell ABIMO entwickelt (Glugla & Fürtig, 1997). Vorläufer dieses Modells war das in den 80er Jahren entwickelte Großrechnermodell RASTER, das auf Rasterbasis arbeitete und das über Jahrzehnte für die DDR-weite Anwendung im Lockergesteinsbereich Anwendung fand (LUA, 2000). Im Rahmen der genannten Kooperation ging es darum das Programm ABIMO so weiter zu entwickeln bzw. umzuformen, dass
-	es die besonderen Verhältnisse des urbanen Wasserhaushaltes besser abbildet als das ursprüngliche Modell (z.B. Verdunstungsverhältnisse verschiedener städtischer Oberflächen und verschiedenes Sickerverhalten unterschiedlich versiegelter Flächen), 
-	die im Rahmen des Umweltatlas erarbeiteten umfangreichen und detaillierten Datengrundlagen zielorientiert eingebunden werden können (z.B. detaillierte Landnutzungs- und Versiegelungsdaten und Daten zu Art und Anschlussgrad der Kanalisation),
-	es in der Lage sein sollte, Daten auf der räumlichen Basis des speziellen Block- und Blockteilflächensystems des Informationssystems Stadt und Umwelt (ISU) zu berechnen,
-	die Parameter Ertragsklasse, Beregnung, Durchwurzelungstiefe, Wachstumstage anhand von Nutzung bzw. Flächentyp des ISU einbezogen werden sowie
-	es auf Windows – Systemen lauffähig sein sollte.

Ergebnis war eine 1996 fertig gestellte Software mit dem Namen ABIMO 2.10. Dieses Programm fand dann unmittelbare Anwendung mit den damals erstmalig für Gesamt-Berlin im Rahmen des Informationssystems Stadt und Umwelt (ISU) der Senatsverwaltung erarbeiteten Eingangsdaten. Die Ergebnisse wurden im ersten Gesamtberliner Umweltatlas 1999 veröffentlicht (SenStadtUmTech, 1999). Eine gemeinsame Veröffentlichung dokumentierte die Arbeiten sowohl zum Modell als auch zu den Eingangsdaten und den Ergebnissen (Glugla, Goedecke, Wessolek & Fürtig, 1999). 
Eine Weiterentwicklung des rasterbezogenen ABIMO gab es insbesondere im Rahmen der Arbeiten zum [Hydrologischen Atlas Deutschland](https://geoportal.bafg.de/mapapps/resources/apps/HAD/index.html?lang=de). ABIMO wurde von der BfG in diesem Zusammenhang zum Verfahren BAGLUVA weiter entwickelt (Glugla & et.al., 2003) und die Modellergebnisse im Rahmen dieses Atlasses veröffentlicht. 

Die Berliner Variante des Programmes ist letztlich sehr auf die speziellen Verhältnisse der Datenbereitstellung auf der Basis der Block- und Blockteilflächen im Rahmen des ISU und insgesamt eher auf urbane Verhältnisse ausgerichtet. Deutschlandweit fand diese Variante daher keine Anwendung, da später auch BAGLUVA mit einigen verbesserten Koeffizienten und Faktoren, die besonders den ländlichen Raum betreffen, eine weitere Verbreitung fand. ABIMO in seiner Berliner Version wurde hingegen in gewissem Umfang von anderen Städten erfolgreich eingesetzt. Dafür wurden die Datengrundlagen so umgeformt, dass sie die erforderlichen Datenstrukturen aufwiesen. Zu nennen sind hier u.a. die Anwendungen in Dresden (Kluck, 2012), Leipzig (Haase, 2008) und Ho-Chi-Minh-Stadt (Goedecke & Rujner, 2016). Die flächendeckende Anwendung im Land Brandenburg basiert hingegen auf der unveränderten BfG-Variante der ABIMO – Software (LUA, 2000).

Das Programm ist von der Berliner Senatsverwaltung seit 1997 mehrmals den sich wandelnden Erfordernissen angepasst worden, letztmalig 2009. Programmkern und wissenschaftliche Grundlagen blieben jedoch unangetastet. Die Veränderungen und Anpassungen sind in drei Dateien dokumentiert, die dem Programm beigefügt sind (versionChanges_2_ti_3.txt, versionChanges_3_to_3_1.txt, versionChanges_3_1_to_3_2.txt).

2005/2006 erfolgte die Erarbeitung der Version 3 (auf der Basis der Version 2.1) mit folgenden wesentlichen Änderungen: 
-	Lauffähigkeit unter Windows XP wurde hergestellt,
-	Vor der Berechnung wird geprüft ob es sich bei der Quelldatei um einen validen File handelt,
-	Die bebaut versiegelten Flächen werden jetzt korrekt berücksichtigt,
-	Zwei neue Baustrukturtypen wurden eingeführt,,
-	Die Verdunstung wird als zusätzlicher Wert in die Ausgabedatei übernommen,
-	weitere kleinere Anpassungen.

2008 erfolgte die Erarbeitung der Version 3.1 mit folgenden wesentlichen Änderungen
-	in der Konfigurationsdatei config.xml ist es möglich für alle Flächentypen die Beregnungshöhe(BER) auf 0 zu erzwingen.

2009 erfolgte die Erarbeitung der Version 3.2 mit folgenden wesentlichen Änderungen
-	Korrektur der Berechnung der "VERDUNSTUNG",
-	Der Niederschlagskorrekturfaktor (NIEDKORRF) ist über die XML Datei einstellbar.
-	Die Verdunstungswerte ETP, ETPS und EG sind für Bezirke in der config.XML Datei einstellbar.

## Weiterführende Handreichungen und Anleitungen für Anwendende in Berlin

**Goedecke, M., Gerstenberg, J., Haag, L. 2019:**
Wasserhaushaltsmodell Berlin ABIMO 3.2 – Handreichung für Anwendende, Technische Anleitung zur Aufbereitung von Datengrundlagen sowie Dokumentation von Methoden und Berechnungsergebnissen des auf Berliner Verhältnisse angepassten blockbezogenen Niederschlags – Abflussmodell ABIMO der Bundesanstalt für Gewässerkunde, 2006 bis 2019, Senatsverwaltung für Stadtentwicklung und Wohnen Berlin, Stand 15.04.2020.
Download:
<https://www.berlin.de/umweltatlas/_assets/literatur/goedecke_et_al_abimo2019_doku.pdf>

**Rachimow, C., Rachimow, M. 2009:**
ABIMO3.2 – Abflußbildungsmodell. Algorithmus zum BAGROV-GLUGLA-Verfahren für die Berechnung langjähriger Mittelwerte des Wasserhaushalts (Version 3.2). Beschreibung des Verfahrens zur Behandlung grundstücksbezogener Daten.
Download:
<https://www.berlin.de/umweltatlas/_assets/literatur/rachimow_rachimow_abimo3_2_2009.pdf> 

## Literatur
Glugla, D., & et.al. (2003): Wasserhaushaltsverfahren zur Berechnung vieljähriger Mittelwerte der tatsächlichen Verdunstung und desGesamtabflusses - BAGLUVA. Bundesanstalt für Gewässerkunde. Koblenz: BfG-Bericht Nr. 1342.

Glugla, G., & Fürtig, G. (1997): Dokumentation zur Anwendung des Rechenprogramms ABIMO. Berlin: Bundesanstalt für Gewässerkunde.

Glugla, G. & König, B. (1989): Der mikrorechnergestützte Arbeitsplatz Grundwasserdargebot. Wasserwirtschaft-Wassertechnik, 39.Jg.

Glugla, G., Goedecke, M., Wessolek, G., & Fürtig, G. (Januar 1999); Langjährige Abflußbildung und Wasserhaushalt im urbanen Gebiet Berlin. Wasserwirtschaft, S. 34-42.

Goedecke, M., & Gerstenberg, J. (2013): Datengrundlagen aus dem Informationssystem Stadt und Umwelt für das Niederschlags - Abflussmodell ABIMO der Bundesanstalt für Gewässerkunde. Senatsverwaltung für Stadtentwicklung und Umwelt Berlin, Stand: 01.10.2014. Berlin.

Goedecke, M., & Rujner, H. (2016): Urban Water Management: Spatial Assessment of the Urban Water Balance. In A. Katzschner, M. Waibel, D.Schwede, L. Katzschner, M. Schmidt, & H. Storch, Sustainable Ho Chi Minh City: Climate Policies for Emerging Mega Cities (S. 133 - 150). Springer. 

Haase, D. (2008): Modelling the Effects and Use Change on the Water Balance. In AGD Landscape & Environment 2 (S. 143- 159). Abgerufen am 05. 12 2019
Internet: <https://www.researchgate.net/publication/228450097_Modelling_the_effects_of_long-term_urban_land_use_change_on_the_water_balance> (Zugriff: 03.05.2022)

Kluck, G. (2012): Modellierung des Wasserhaushaltes für den quartären Elbtalaquifer für das Stadtgebiet von Dresden - Diplomarbeit. Freiberg. 

LUA. (2000): Flächendeckende Modellierung von Wasserhaushaltsgrößen für das Land Brandenburg. Landesumweltamt Brandenburg, Potsdam. Von https://lfu.brandenburg.de/cms/media.php/lbm1.a.3310.de/luabd27_1.pdf abgerufen 

Rachimow, C., & Rachimow, M. (2009): ABIMO 3.2 - Abflußbildungsmodell - Algorithmus zum BAGROV-GLUGLA-Verfahren für die Berechnung langjähriger Mittelwerte des Wasserhaushalts. Werder. 

SenStadtUmTech. (1999): Umweltatlas Berlin - 02.13 Oberflächenabfluss, Versickerung und Gesamtabfluss aus Niederschlägen (Bd. 1). (U. u. SenStadtUmTech (Senatsverwaltung für Stadtentwicklung, Hrsg.) Berlin.
