#include "main.h"

void	test_u_convert(void) {
# define UNSIGNED_TEST(s_nbr) do {					\
		unsigned int _unb = 0;					\
		unsigned int _expected = (unsigned int)strtoul(s_nbr, NULL, 10); \
									\
		test(s_nbr, "%u", &_unb);				\
		if (_unb != _expected) {				\
		dprintf(2, "expected: %u, got: %u\n", _expected, _unb); \
			}						\
		assert(_unb == _expected);				\
		} while (0);

			 UNSIGNED_TEST("0");
			 UNSIGNED_TEST("1");
			 UNSIGNED_TEST("2");
			 UNSIGNED_TEST("3");
			 UNSIGNED_TEST("4");
			 UNSIGNED_TEST("5");
			 UNSIGNED_TEST("6");
			 UNSIGNED_TEST("7");
			 UNSIGNED_TEST("8");
			 UNSIGNED_TEST("9");
			 UNSIGNED_TEST("10");
			 UNSIGNED_TEST("11");
			 UNSIGNED_TEST("12");
			 UNSIGNED_TEST("13");
			 UNSIGNED_TEST("14");
			 UNSIGNED_TEST("15");
			 UNSIGNED_TEST("16");
			 UNSIGNED_TEST("17");
			 UNSIGNED_TEST("18");
			 UNSIGNED_TEST("19");
			 UNSIGNED_TEST("20");
			 UNSIGNED_TEST("21");
			 UNSIGNED_TEST("22");
			 UNSIGNED_TEST("23");
			 UNSIGNED_TEST("24");
			 UNSIGNED_TEST("25");
			 UNSIGNED_TEST("26");
			 UNSIGNED_TEST("27");
			 UNSIGNED_TEST("28");
			 UNSIGNED_TEST("29");
			 UNSIGNED_TEST("30");
			 UNSIGNED_TEST("31");
			 UNSIGNED_TEST("32");
			 UNSIGNED_TEST("33");
			 UNSIGNED_TEST("34");
			 UNSIGNED_TEST("35");
			 UNSIGNED_TEST("36");
			 UNSIGNED_TEST("37");
			 UNSIGNED_TEST("38");
			 UNSIGNED_TEST("39");
			 UNSIGNED_TEST("40");
			 UNSIGNED_TEST("41");
			 UNSIGNED_TEST("42");
			 UNSIGNED_TEST("43");
			 UNSIGNED_TEST("44");
			 UNSIGNED_TEST("45");
			 UNSIGNED_TEST("46");
			 UNSIGNED_TEST("47");
			 UNSIGNED_TEST("48");
			 UNSIGNED_TEST("49");
			 UNSIGNED_TEST("50");
			 UNSIGNED_TEST("51");
			 UNSIGNED_TEST("52");
			 UNSIGNED_TEST("53");
			 UNSIGNED_TEST("54");
			 UNSIGNED_TEST("55");
			 UNSIGNED_TEST("56");
			 UNSIGNED_TEST("57");
			 UNSIGNED_TEST("58");
			 UNSIGNED_TEST("59");
			 UNSIGNED_TEST("60");
			 UNSIGNED_TEST("61");
			 UNSIGNED_TEST("62");
			 UNSIGNED_TEST("63");
			 UNSIGNED_TEST("64");
			 UNSIGNED_TEST("65");
			 UNSIGNED_TEST("66");
			 UNSIGNED_TEST("67");
			 UNSIGNED_TEST("68");
			 UNSIGNED_TEST("69");
			 UNSIGNED_TEST("70");
			 UNSIGNED_TEST("71");
			 UNSIGNED_TEST("72");
			 UNSIGNED_TEST("73");
			 UNSIGNED_TEST("74");
			 UNSIGNED_TEST("75");
			 UNSIGNED_TEST("76");
			 UNSIGNED_TEST("77");
			 UNSIGNED_TEST("78");
			 UNSIGNED_TEST("79");
			 UNSIGNED_TEST("80");
			 UNSIGNED_TEST("81");
			 UNSIGNED_TEST("82");
			 UNSIGNED_TEST("83");
			 UNSIGNED_TEST("84");
			 UNSIGNED_TEST("85");
			 UNSIGNED_TEST("86");
			 UNSIGNED_TEST("87");
			 UNSIGNED_TEST("88");
			 UNSIGNED_TEST("89");
			 UNSIGNED_TEST("90");
			 UNSIGNED_TEST("91");
			 UNSIGNED_TEST("92");
			 UNSIGNED_TEST("93");
			 UNSIGNED_TEST("94");
			 UNSIGNED_TEST("95");
			 UNSIGNED_TEST("96");
			 UNSIGNED_TEST("97");
			 UNSIGNED_TEST("98");
			 UNSIGNED_TEST("99");
			 UNSIGNED_TEST("100");
			 UNSIGNED_TEST("101");
			 UNSIGNED_TEST("102");
			 UNSIGNED_TEST("103");
			 UNSIGNED_TEST("104");
			 UNSIGNED_TEST("105");
			 UNSIGNED_TEST("106");
			 UNSIGNED_TEST("107");
			 UNSIGNED_TEST("108");
			 UNSIGNED_TEST("109");
			 UNSIGNED_TEST("110");
			 UNSIGNED_TEST("111");
			 UNSIGNED_TEST("112");
			 UNSIGNED_TEST("113");
			 UNSIGNED_TEST("114");
			 UNSIGNED_TEST("115");
			 UNSIGNED_TEST("116");
			 UNSIGNED_TEST("117");
			 UNSIGNED_TEST("118");
			 UNSIGNED_TEST("119");
			 UNSIGNED_TEST("120");
			 UNSIGNED_TEST("121");
			 UNSIGNED_TEST("122");
			 UNSIGNED_TEST("123");
			 UNSIGNED_TEST("124");
			 UNSIGNED_TEST("125");
			 UNSIGNED_TEST("126");
			 UNSIGNED_TEST("127");
			 UNSIGNED_TEST("128");
			 UNSIGNED_TEST("129");
			 UNSIGNED_TEST("130");
			 UNSIGNED_TEST("131");
			 UNSIGNED_TEST("132");
			 UNSIGNED_TEST("133");
			 UNSIGNED_TEST("134");
			 UNSIGNED_TEST("135");
			 UNSIGNED_TEST("136");
			 UNSIGNED_TEST("137");
			 UNSIGNED_TEST("138");
			 UNSIGNED_TEST("139");
			 UNSIGNED_TEST("140");
			 UNSIGNED_TEST("141");
			 UNSIGNED_TEST("142");
			 UNSIGNED_TEST("143");
			 UNSIGNED_TEST("144");
			 UNSIGNED_TEST("145");
			 UNSIGNED_TEST("146");
			 UNSIGNED_TEST("147");
			 UNSIGNED_TEST("148");
			 UNSIGNED_TEST("149");
			 UNSIGNED_TEST("150");
			 UNSIGNED_TEST("151");
			 UNSIGNED_TEST("152");
			 UNSIGNED_TEST("153");
			 UNSIGNED_TEST("154");
			 UNSIGNED_TEST("155");
			 UNSIGNED_TEST("156");
			 UNSIGNED_TEST("157");
			 UNSIGNED_TEST("158");
			 UNSIGNED_TEST("159");
			 UNSIGNED_TEST("160");
			 UNSIGNED_TEST("161");
			 UNSIGNED_TEST("162");
			 UNSIGNED_TEST("163");
			 UNSIGNED_TEST("164");
			 UNSIGNED_TEST("165");
			 UNSIGNED_TEST("166");
			 UNSIGNED_TEST("167");
			 UNSIGNED_TEST("168");
			 UNSIGNED_TEST("169");
			 UNSIGNED_TEST("170");
			 UNSIGNED_TEST("171");
			 UNSIGNED_TEST("172");
			 UNSIGNED_TEST("173");
			 UNSIGNED_TEST("174");
			 UNSIGNED_TEST("175");
			 UNSIGNED_TEST("176");
			 UNSIGNED_TEST("177");
			 UNSIGNED_TEST("178");
			 UNSIGNED_TEST("179");
			 UNSIGNED_TEST("180");
			 UNSIGNED_TEST("181");
			 UNSIGNED_TEST("182");
			 UNSIGNED_TEST("183");
			 UNSIGNED_TEST("184");
			 UNSIGNED_TEST("185");
			 UNSIGNED_TEST("186");
			 UNSIGNED_TEST("187");
			 UNSIGNED_TEST("188");
			 UNSIGNED_TEST("189");
			 UNSIGNED_TEST("190");
			 UNSIGNED_TEST("191");
			 UNSIGNED_TEST("192");
			 UNSIGNED_TEST("193");
			 UNSIGNED_TEST("194");
			 UNSIGNED_TEST("195");
			 UNSIGNED_TEST("196");
			 UNSIGNED_TEST("197");
			 UNSIGNED_TEST("198");
			 UNSIGNED_TEST("199");
			 UNSIGNED_TEST("200");
			 UNSIGNED_TEST("201");
			 UNSIGNED_TEST("202");
			 UNSIGNED_TEST("203");
			 UNSIGNED_TEST("204");
			 UNSIGNED_TEST("205");
			 UNSIGNED_TEST("206");
			 UNSIGNED_TEST("207");
			 UNSIGNED_TEST("208");
			 UNSIGNED_TEST("209");
			 UNSIGNED_TEST("210");
			 UNSIGNED_TEST("211");
			 UNSIGNED_TEST("212");
			 UNSIGNED_TEST("213");
			 UNSIGNED_TEST("214");
			 UNSIGNED_TEST("215");
			 UNSIGNED_TEST("216");
			 UNSIGNED_TEST("217");
			 UNSIGNED_TEST("218");
			 UNSIGNED_TEST("219");
			 UNSIGNED_TEST("220");
			 UNSIGNED_TEST("221");
			 UNSIGNED_TEST("222");
			 UNSIGNED_TEST("223");
			 UNSIGNED_TEST("224");
			 UNSIGNED_TEST("225");
			 UNSIGNED_TEST("226");
			 UNSIGNED_TEST("227");
			 UNSIGNED_TEST("228");
			 UNSIGNED_TEST("229");
			 UNSIGNED_TEST("230");
			 UNSIGNED_TEST("231");
			 UNSIGNED_TEST("232");
			 UNSIGNED_TEST("233");
			 UNSIGNED_TEST("234");
			 UNSIGNED_TEST("235");
			 UNSIGNED_TEST("236");
			 UNSIGNED_TEST("237");
			 UNSIGNED_TEST("238");
			 UNSIGNED_TEST("239");
			 UNSIGNED_TEST("240");
			 UNSIGNED_TEST("241");
			 UNSIGNED_TEST("242");
			 UNSIGNED_TEST("243");
			 UNSIGNED_TEST("244");
			 UNSIGNED_TEST("245");
			 UNSIGNED_TEST("246");
			 UNSIGNED_TEST("247");
			 UNSIGNED_TEST("248");
			 UNSIGNED_TEST("249");
			 UNSIGNED_TEST("250");
			 UNSIGNED_TEST("251");
			 UNSIGNED_TEST("252");
			 UNSIGNED_TEST("253");
			 UNSIGNED_TEST("254");
			 UNSIGNED_TEST("255");
			 UNSIGNED_TEST("256");
			 UNSIGNED_TEST("257");
			 UNSIGNED_TEST("258");
			 UNSIGNED_TEST("259");
			 UNSIGNED_TEST("260");
			 UNSIGNED_TEST("261");
			 UNSIGNED_TEST("262");
			 UNSIGNED_TEST("263");
			 UNSIGNED_TEST("264");
			 UNSIGNED_TEST("265");
			 UNSIGNED_TEST("266");
			 UNSIGNED_TEST("267");
			 UNSIGNED_TEST("268");
			 UNSIGNED_TEST("269");
			 UNSIGNED_TEST("270");
			 UNSIGNED_TEST("271");
			 UNSIGNED_TEST("272");
			 UNSIGNED_TEST("273");
			 UNSIGNED_TEST("274");
			 UNSIGNED_TEST("275");
			 UNSIGNED_TEST("276");
			 UNSIGNED_TEST("277");
			 UNSIGNED_TEST("278");
			 UNSIGNED_TEST("279");
			 UNSIGNED_TEST("280");
			 UNSIGNED_TEST("281");
			 UNSIGNED_TEST("282");
			 UNSIGNED_TEST("283");
			 UNSIGNED_TEST("284");
			 UNSIGNED_TEST("285");
			 UNSIGNED_TEST("286");
			 UNSIGNED_TEST("287");
			 UNSIGNED_TEST("288");
			 UNSIGNED_TEST("289");
			 UNSIGNED_TEST("290");
			 UNSIGNED_TEST("291");
			 UNSIGNED_TEST("292");
			 UNSIGNED_TEST("293");
			 UNSIGNED_TEST("294");
			 UNSIGNED_TEST("295");
			 UNSIGNED_TEST("296");
			 UNSIGNED_TEST("297");
			 UNSIGNED_TEST("298");
			 UNSIGNED_TEST("299");
			 UNSIGNED_TEST("300");
			 UNSIGNED_TEST("301");
			 UNSIGNED_TEST("302");
			 UNSIGNED_TEST("303");
			 UNSIGNED_TEST("304");
			 UNSIGNED_TEST("305");
			 UNSIGNED_TEST("306");
			 UNSIGNED_TEST("307");
			 UNSIGNED_TEST("308");
			 UNSIGNED_TEST("309");
			 UNSIGNED_TEST("310");
			 UNSIGNED_TEST("311");
			 UNSIGNED_TEST("312");
			 UNSIGNED_TEST("313");
			 UNSIGNED_TEST("314");
			 UNSIGNED_TEST("315");
			 UNSIGNED_TEST("316");
			 UNSIGNED_TEST("317");
			 UNSIGNED_TEST("318");
			 UNSIGNED_TEST("319");
			 UNSIGNED_TEST("320");
			 UNSIGNED_TEST("321");
			 UNSIGNED_TEST("322");
			 UNSIGNED_TEST("323");
			 UNSIGNED_TEST("324");
			 UNSIGNED_TEST("325");
			 UNSIGNED_TEST("326");
			 UNSIGNED_TEST("327");
			 UNSIGNED_TEST("328");
			 UNSIGNED_TEST("329");
			 UNSIGNED_TEST("330");
			 UNSIGNED_TEST("331");
			 UNSIGNED_TEST("332");
			 UNSIGNED_TEST("333");
			 UNSIGNED_TEST("334");
			 UNSIGNED_TEST("335");
			 UNSIGNED_TEST("336");
			 UNSIGNED_TEST("337");
			 UNSIGNED_TEST("338");
			 UNSIGNED_TEST("339");
			 UNSIGNED_TEST("340");
			 UNSIGNED_TEST("341");
			 UNSIGNED_TEST("342");
			 UNSIGNED_TEST("343");
			 UNSIGNED_TEST("344");
			 UNSIGNED_TEST("345");
			 UNSIGNED_TEST("346");
			 UNSIGNED_TEST("347");
			 UNSIGNED_TEST("348");
			 UNSIGNED_TEST("349");
			 UNSIGNED_TEST("350");
			 UNSIGNED_TEST("351");
			 UNSIGNED_TEST("352");
			 UNSIGNED_TEST("353");
			 UNSIGNED_TEST("354");
			 UNSIGNED_TEST("355");
			 UNSIGNED_TEST("356");
			 UNSIGNED_TEST("357");
			 UNSIGNED_TEST("358");
			 UNSIGNED_TEST("359");
			 UNSIGNED_TEST("360");
			 UNSIGNED_TEST("361");
			 UNSIGNED_TEST("362");
			 UNSIGNED_TEST("363");
			 UNSIGNED_TEST("364");
			 UNSIGNED_TEST("365");
			 UNSIGNED_TEST("366");
			 UNSIGNED_TEST("367");
			 UNSIGNED_TEST("368");
			 UNSIGNED_TEST("369");
			 UNSIGNED_TEST("370");
			 UNSIGNED_TEST("371");
			 UNSIGNED_TEST("372");
			 UNSIGNED_TEST("373");
			 UNSIGNED_TEST("374");
			 UNSIGNED_TEST("375");
			 UNSIGNED_TEST("376");
			 UNSIGNED_TEST("377");
			 UNSIGNED_TEST("378");
			 UNSIGNED_TEST("379");
			 UNSIGNED_TEST("380");
			 UNSIGNED_TEST("381");
			 UNSIGNED_TEST("382");
			 UNSIGNED_TEST("383");
			 UNSIGNED_TEST("384");
			 UNSIGNED_TEST("385");
			 UNSIGNED_TEST("386");
			 UNSIGNED_TEST("387");
			 UNSIGNED_TEST("388");
			 UNSIGNED_TEST("389");
			 UNSIGNED_TEST("390");
			 UNSIGNED_TEST("391");
			 UNSIGNED_TEST("392");
			 UNSIGNED_TEST("393");
			 UNSIGNED_TEST("394");
			 UNSIGNED_TEST("395");
			 UNSIGNED_TEST("396");
			 UNSIGNED_TEST("397");
			 UNSIGNED_TEST("398");
			 UNSIGNED_TEST("399");
			 UNSIGNED_TEST("400");
			 UNSIGNED_TEST("401");
			 UNSIGNED_TEST("402");
			 UNSIGNED_TEST("403");
			 UNSIGNED_TEST("404");
			 UNSIGNED_TEST("405");
			 UNSIGNED_TEST("406");
			 UNSIGNED_TEST("407");
			 UNSIGNED_TEST("408");
			 UNSIGNED_TEST("409");
			 UNSIGNED_TEST("410");
			 UNSIGNED_TEST("411");
			 UNSIGNED_TEST("412");
			 UNSIGNED_TEST("413");
			 UNSIGNED_TEST("414");
			 UNSIGNED_TEST("415");
			 UNSIGNED_TEST("416");
			 UNSIGNED_TEST("417");
			 UNSIGNED_TEST("418");
			 UNSIGNED_TEST("419");
			 UNSIGNED_TEST("420");
			 UNSIGNED_TEST("421");
			 UNSIGNED_TEST("422");
			 UNSIGNED_TEST("423");
			 UNSIGNED_TEST("424");
			 UNSIGNED_TEST("425");
			 UNSIGNED_TEST("426");
			 UNSIGNED_TEST("427");
			 UNSIGNED_TEST("428");
			 UNSIGNED_TEST("429");
			 UNSIGNED_TEST("430");
			 UNSIGNED_TEST("431");
			 UNSIGNED_TEST("432");
			 UNSIGNED_TEST("433");
			 UNSIGNED_TEST("434");
			 UNSIGNED_TEST("435");
			 UNSIGNED_TEST("436");
			 UNSIGNED_TEST("437");
			 UNSIGNED_TEST("438");
			 UNSIGNED_TEST("439");
			 UNSIGNED_TEST("440");
			 UNSIGNED_TEST("441");
			 UNSIGNED_TEST("442");
			 UNSIGNED_TEST("443");
			 UNSIGNED_TEST("444");
			 UNSIGNED_TEST("445");
			 UNSIGNED_TEST("446");
			 UNSIGNED_TEST("447");
			 UNSIGNED_TEST("448");
			 UNSIGNED_TEST("449");
			 UNSIGNED_TEST("450");
			 UNSIGNED_TEST("451");
			 UNSIGNED_TEST("452");
			 UNSIGNED_TEST("453");
			 UNSIGNED_TEST("454");
			 UNSIGNED_TEST("455");
			 UNSIGNED_TEST("456");
			 UNSIGNED_TEST("457");
			 UNSIGNED_TEST("458");
			 UNSIGNED_TEST("459");
			 UNSIGNED_TEST("460");
			 UNSIGNED_TEST("461");
			 UNSIGNED_TEST("462");
			 UNSIGNED_TEST("463");
			 UNSIGNED_TEST("464");
			 UNSIGNED_TEST("465");
			 UNSIGNED_TEST("466");
			 UNSIGNED_TEST("467");
			 UNSIGNED_TEST("468");
			 UNSIGNED_TEST("469");
			 UNSIGNED_TEST("470");
			 UNSIGNED_TEST("471");
			 UNSIGNED_TEST("472");
			 UNSIGNED_TEST("473");
			 UNSIGNED_TEST("474");
			 UNSIGNED_TEST("475");
			 UNSIGNED_TEST("476");
			 UNSIGNED_TEST("477");
			 UNSIGNED_TEST("478");
			 UNSIGNED_TEST("479");
			 UNSIGNED_TEST("480");
			 UNSIGNED_TEST("481");
			 UNSIGNED_TEST("482");
			 UNSIGNED_TEST("483");
			 UNSIGNED_TEST("484");
			 UNSIGNED_TEST("485");
			 UNSIGNED_TEST("486");
			 UNSIGNED_TEST("487");
			 UNSIGNED_TEST("488");
			 UNSIGNED_TEST("489");
			 UNSIGNED_TEST("490");
			 UNSIGNED_TEST("491");
			 UNSIGNED_TEST("492");
			 UNSIGNED_TEST("493");
			 UNSIGNED_TEST("494");
			 UNSIGNED_TEST("495");
			 UNSIGNED_TEST("496");
			 UNSIGNED_TEST("497");
			 UNSIGNED_TEST("498");
			 UNSIGNED_TEST("499");
			 UNSIGNED_TEST("500");
			 UNSIGNED_TEST("501");
			 UNSIGNED_TEST("502");
			 UNSIGNED_TEST("503");
			 UNSIGNED_TEST("504");
			 UNSIGNED_TEST("505");
			 UNSIGNED_TEST("506");
			 UNSIGNED_TEST("507");
			 UNSIGNED_TEST("508");
			 UNSIGNED_TEST("509");
			 UNSIGNED_TEST("510");
			 UNSIGNED_TEST("511");
			 UNSIGNED_TEST("512");
			 UNSIGNED_TEST("513");
			 UNSIGNED_TEST("514");
			 UNSIGNED_TEST("515");
			 UNSIGNED_TEST("516");
			 UNSIGNED_TEST("517");
			 UNSIGNED_TEST("518");
			 UNSIGNED_TEST("519");
			 UNSIGNED_TEST("520");
			 UNSIGNED_TEST("521");
			 UNSIGNED_TEST("522");
			 UNSIGNED_TEST("523");
			 UNSIGNED_TEST("524");
			 UNSIGNED_TEST("525");
			 UNSIGNED_TEST("526");
			 UNSIGNED_TEST("527");
			 UNSIGNED_TEST("528");
			 UNSIGNED_TEST("529");
			 UNSIGNED_TEST("530");
			 UNSIGNED_TEST("531");
			 UNSIGNED_TEST("532");
			 UNSIGNED_TEST("533");
			 UNSIGNED_TEST("534");
			 UNSIGNED_TEST("535");
			 UNSIGNED_TEST("536");
			 UNSIGNED_TEST("537");
			 UNSIGNED_TEST("538");
			 UNSIGNED_TEST("539");
			 UNSIGNED_TEST("540");
			 UNSIGNED_TEST("541");
			 UNSIGNED_TEST("542");
			 UNSIGNED_TEST("543");
			 UNSIGNED_TEST("544");
			 UNSIGNED_TEST("545");
			 UNSIGNED_TEST("546");
			 UNSIGNED_TEST("547");
			 UNSIGNED_TEST("548");
			 UNSIGNED_TEST("549");
			 UNSIGNED_TEST("550");
			 UNSIGNED_TEST("551");
			 UNSIGNED_TEST("552");
			 UNSIGNED_TEST("553");
			 UNSIGNED_TEST("554");
			 UNSIGNED_TEST("555");
			 UNSIGNED_TEST("556");
			 UNSIGNED_TEST("557");
			 UNSIGNED_TEST("558");
			 UNSIGNED_TEST("559");
			 UNSIGNED_TEST("560");
			 UNSIGNED_TEST("561");
			 UNSIGNED_TEST("562");
			 UNSIGNED_TEST("563");
			 UNSIGNED_TEST("564");
			 UNSIGNED_TEST("565");
			 UNSIGNED_TEST("566");
			 UNSIGNED_TEST("567");
	UNSIGNED_TEST("568");
	UNSIGNED_TEST("569");
	UNSIGNED_TEST("570");
	UNSIGNED_TEST("571");
	UNSIGNED_TEST("572");
	UNSIGNED_TEST("573");
	UNSIGNED_TEST("574");
	UNSIGNED_TEST("575");
	UNSIGNED_TEST("576");
	UNSIGNED_TEST("577");
	UNSIGNED_TEST("578");
	UNSIGNED_TEST("579");
	UNSIGNED_TEST("580");
	UNSIGNED_TEST("581");
	UNSIGNED_TEST("582");
	UNSIGNED_TEST("583");
	UNSIGNED_TEST("584");
	UNSIGNED_TEST("585");
	UNSIGNED_TEST("586");
	UNSIGNED_TEST("587");
	UNSIGNED_TEST("588");
	UNSIGNED_TEST("589");
	UNSIGNED_TEST("590");
	UNSIGNED_TEST("591");
	UNSIGNED_TEST("592");
	UNSIGNED_TEST("593");
	UNSIGNED_TEST("594");
	UNSIGNED_TEST("595");
	UNSIGNED_TEST("596");
	UNSIGNED_TEST("597");
	UNSIGNED_TEST("598");
	UNSIGNED_TEST("599");
	UNSIGNED_TEST("600");
	UNSIGNED_TEST("601");
	UNSIGNED_TEST("602");
	UNSIGNED_TEST("603");
	UNSIGNED_TEST("604");
	UNSIGNED_TEST("605");
	UNSIGNED_TEST("606");
	UNSIGNED_TEST("607");
	UNSIGNED_TEST("608");
	UNSIGNED_TEST("609");
	UNSIGNED_TEST("610");
	UNSIGNED_TEST("611");
	UNSIGNED_TEST("612");
	UNSIGNED_TEST("613");
	UNSIGNED_TEST("614");
	UNSIGNED_TEST("615");
	UNSIGNED_TEST("616");
	UNSIGNED_TEST("617");
	UNSIGNED_TEST("618");
	UNSIGNED_TEST("619");
	UNSIGNED_TEST("620");
	UNSIGNED_TEST("621");
	UNSIGNED_TEST("622");
	UNSIGNED_TEST("623");
	UNSIGNED_TEST("624");
	UNSIGNED_TEST("625");
	UNSIGNED_TEST("626");
	UNSIGNED_TEST("627");
	UNSIGNED_TEST("628");
	UNSIGNED_TEST("629");
	UNSIGNED_TEST("630");
	UNSIGNED_TEST("631");
	UNSIGNED_TEST("632");
	UNSIGNED_TEST("633");
	UNSIGNED_TEST("634");
	UNSIGNED_TEST("635");
	UNSIGNED_TEST("636");
	UNSIGNED_TEST("637");
	UNSIGNED_TEST("638");
	UNSIGNED_TEST("639");
	UNSIGNED_TEST("640");
	UNSIGNED_TEST("641");
	UNSIGNED_TEST("642");
	UNSIGNED_TEST("643");
	UNSIGNED_TEST("644");
	UNSIGNED_TEST("645");
	UNSIGNED_TEST("646");
	UNSIGNED_TEST("647");
	UNSIGNED_TEST("648");
	UNSIGNED_TEST("649");
	UNSIGNED_TEST("650");
	UNSIGNED_TEST("651");
	UNSIGNED_TEST("652");
	UNSIGNED_TEST("653");
	UNSIGNED_TEST("654");
	UNSIGNED_TEST("655");
	UNSIGNED_TEST("656");
	UNSIGNED_TEST("657");
	UNSIGNED_TEST("658");
	UNSIGNED_TEST("659");
	UNSIGNED_TEST("660");
	UNSIGNED_TEST("661");
	UNSIGNED_TEST("662");
	UNSIGNED_TEST("663");
	UNSIGNED_TEST("664");
	UNSIGNED_TEST("665");
	UNSIGNED_TEST("666");
	UNSIGNED_TEST("667");
	UNSIGNED_TEST("668");
	UNSIGNED_TEST("669");
	UNSIGNED_TEST("670");
	UNSIGNED_TEST("671");
	UNSIGNED_TEST("672");
	UNSIGNED_TEST("673");
	UNSIGNED_TEST("674");
	UNSIGNED_TEST("675");
	UNSIGNED_TEST("676");
	UNSIGNED_TEST("677");
	UNSIGNED_TEST("678");
	UNSIGNED_TEST("679");
	UNSIGNED_TEST("680");
	UNSIGNED_TEST("681");
	UNSIGNED_TEST("682");
	UNSIGNED_TEST("683");
	UNSIGNED_TEST("684");
	UNSIGNED_TEST("685");
	UNSIGNED_TEST("686");
	UNSIGNED_TEST("687");
	UNSIGNED_TEST("688");
	UNSIGNED_TEST("689");
	UNSIGNED_TEST("690");
	UNSIGNED_TEST("691");
	UNSIGNED_TEST("692");
	UNSIGNED_TEST("693");
	UNSIGNED_TEST("694");
	UNSIGNED_TEST("695");
	UNSIGNED_TEST("696");
	UNSIGNED_TEST("697");
	UNSIGNED_TEST("698");
			 UNSIGNED_TEST("699");
			 UNSIGNED_TEST("700");
			 UNSIGNED_TEST("701");
			 UNSIGNED_TEST("702");
			 UNSIGNED_TEST("703");
			 UNSIGNED_TEST("704");
			 UNSIGNED_TEST("705");
			 UNSIGNED_TEST("706");
			 UNSIGNED_TEST("707");
			 UNSIGNED_TEST("708");
			 UNSIGNED_TEST("709");
			 UNSIGNED_TEST("710");
			 UNSIGNED_TEST("711");
			 UNSIGNED_TEST("712");
			 UNSIGNED_TEST("713");
			 UNSIGNED_TEST("714");
			 UNSIGNED_TEST("715");
			 UNSIGNED_TEST("716");
			 UNSIGNED_TEST("717");
			 UNSIGNED_TEST("718");
			 UNSIGNED_TEST("719");
			 UNSIGNED_TEST("720");
			 UNSIGNED_TEST("721");
			 UNSIGNED_TEST("722");
			 UNSIGNED_TEST("723");
			 UNSIGNED_TEST("724");
			 UNSIGNED_TEST("725");
			 UNSIGNED_TEST("726");
			 UNSIGNED_TEST("727");
			 UNSIGNED_TEST("728");
			 UNSIGNED_TEST("729");
			 UNSIGNED_TEST("730");
			 UNSIGNED_TEST("731");
			 UNSIGNED_TEST("732");
			 UNSIGNED_TEST("733");
			 UNSIGNED_TEST("734");
			 UNSIGNED_TEST("735");
			 UNSIGNED_TEST("736");
			 UNSIGNED_TEST("737");
			 UNSIGNED_TEST("738");
			 UNSIGNED_TEST("739");
			 UNSIGNED_TEST("740");
			 UNSIGNED_TEST("741");
			 UNSIGNED_TEST("742");
			 UNSIGNED_TEST("743");
			 UNSIGNED_TEST("744");
			 UNSIGNED_TEST("745");
			 UNSIGNED_TEST("746");
			 UNSIGNED_TEST("747");
			 UNSIGNED_TEST("748");
			 UNSIGNED_TEST("749");
			 UNSIGNED_TEST("750");
			 UNSIGNED_TEST("751");
			 UNSIGNED_TEST("752");
			 UNSIGNED_TEST("753");
			 UNSIGNED_TEST("754");
			 UNSIGNED_TEST("755");
			 UNSIGNED_TEST("756");
			 UNSIGNED_TEST("757");
			 UNSIGNED_TEST("758");
			 UNSIGNED_TEST("759");
			 UNSIGNED_TEST("760");
			 UNSIGNED_TEST("761");
			 UNSIGNED_TEST("762");
			 UNSIGNED_TEST("763");
			 UNSIGNED_TEST("764");
			 UNSIGNED_TEST("765");
			 UNSIGNED_TEST("766");
			 UNSIGNED_TEST("767");
			 UNSIGNED_TEST("768");
			 UNSIGNED_TEST("769");
			 UNSIGNED_TEST("770");
			 UNSIGNED_TEST("771");
			 UNSIGNED_TEST("772");
			 UNSIGNED_TEST("773");
			 UNSIGNED_TEST("774");
			 UNSIGNED_TEST("775");
			 UNSIGNED_TEST("776");
			 UNSIGNED_TEST("777");
			 UNSIGNED_TEST("778");
			 UNSIGNED_TEST("779");
			 UNSIGNED_TEST("780");
			 UNSIGNED_TEST("781");
			 UNSIGNED_TEST("782");
			 UNSIGNED_TEST("783");
			 UNSIGNED_TEST("784");
			 UNSIGNED_TEST("785");
			 UNSIGNED_TEST("786");
			 UNSIGNED_TEST("787");
			 UNSIGNED_TEST("788");
			 UNSIGNED_TEST("789");
			 UNSIGNED_TEST("790");
			 UNSIGNED_TEST("791");
			 UNSIGNED_TEST("792");
			 UNSIGNED_TEST("793");
			 UNSIGNED_TEST("794");
			 UNSIGNED_TEST("795");
			 UNSIGNED_TEST("796");
			 UNSIGNED_TEST("797");
			 UNSIGNED_TEST("798");
			 UNSIGNED_TEST("799");
			 UNSIGNED_TEST("800");
			 UNSIGNED_TEST("801");
			 UNSIGNED_TEST("802");
			 UNSIGNED_TEST("803");
			 UNSIGNED_TEST("804");
			 UNSIGNED_TEST("805");
			 UNSIGNED_TEST("806");
			 UNSIGNED_TEST("807");
			 UNSIGNED_TEST("808");
			 UNSIGNED_TEST("809");
			 UNSIGNED_TEST("810");
			 UNSIGNED_TEST("811");
			 UNSIGNED_TEST("812");
			 UNSIGNED_TEST("813");
	UNSIGNED_TEST("814");
	UNSIGNED_TEST("815");
	UNSIGNED_TEST("816");
	UNSIGNED_TEST("817");
	UNSIGNED_TEST("818");
	UNSIGNED_TEST("819");
	UNSIGNED_TEST("820");
	UNSIGNED_TEST("821");
	UNSIGNED_TEST("822");
	UNSIGNED_TEST("823");
	UNSIGNED_TEST("824");
	UNSIGNED_TEST("825");
	UNSIGNED_TEST("826");
	UNSIGNED_TEST("827");
	UNSIGNED_TEST("828");
	UNSIGNED_TEST("829");
	UNSIGNED_TEST("830");
	UNSIGNED_TEST("831");
	UNSIGNED_TEST("832");
	UNSIGNED_TEST("833");
	UNSIGNED_TEST("834");
	UNSIGNED_TEST("835");
	UNSIGNED_TEST("836");
	UNSIGNED_TEST("837");
	UNSIGNED_TEST("838");
	UNSIGNED_TEST("839");
	UNSIGNED_TEST("840");
	UNSIGNED_TEST("841");
	UNSIGNED_TEST("842");
	UNSIGNED_TEST("843");
	UNSIGNED_TEST("844");
	UNSIGNED_TEST("845");
	UNSIGNED_TEST("846");
	UNSIGNED_TEST("847");
	UNSIGNED_TEST("848");
	UNSIGNED_TEST("849");
	UNSIGNED_TEST("850");
	UNSIGNED_TEST("851");
	UNSIGNED_TEST("852");
	UNSIGNED_TEST("853");
	UNSIGNED_TEST("854");
	UNSIGNED_TEST("855");
	UNSIGNED_TEST("856");
	UNSIGNED_TEST("857");
	UNSIGNED_TEST("858");
	UNSIGNED_TEST("859");
	UNSIGNED_TEST("860");
	UNSIGNED_TEST("861");
	UNSIGNED_TEST("862");
	UNSIGNED_TEST("863");
	UNSIGNED_TEST("864");
	UNSIGNED_TEST("865");
	UNSIGNED_TEST("866");
	UNSIGNED_TEST("867");
	UNSIGNED_TEST("868");
	UNSIGNED_TEST("869");
	UNSIGNED_TEST("870");
	UNSIGNED_TEST("871");
	UNSIGNED_TEST("872");
	UNSIGNED_TEST("873");
	UNSIGNED_TEST("874");
	UNSIGNED_TEST("875");
	UNSIGNED_TEST("876");
	UNSIGNED_TEST("877");
	UNSIGNED_TEST("878");
	UNSIGNED_TEST("879");
	UNSIGNED_TEST("880");
	UNSIGNED_TEST("881");
	UNSIGNED_TEST("882");
	UNSIGNED_TEST("883");
	UNSIGNED_TEST("884");
	UNSIGNED_TEST("885");
	UNSIGNED_TEST("886");
	UNSIGNED_TEST("887");
	UNSIGNED_TEST("888");
	UNSIGNED_TEST("889");
	UNSIGNED_TEST("890");
	UNSIGNED_TEST("891");
	UNSIGNED_TEST("892");
	UNSIGNED_TEST("893");
	UNSIGNED_TEST("894");
	UNSIGNED_TEST("895");
	UNSIGNED_TEST("896");
	UNSIGNED_TEST("897");
	UNSIGNED_TEST("898");
	UNSIGNED_TEST("899");
	UNSIGNED_TEST("900");
	UNSIGNED_TEST("901");
	UNSIGNED_TEST("902");
	UNSIGNED_TEST("903");
	UNSIGNED_TEST("904");
	UNSIGNED_TEST("905");
	UNSIGNED_TEST("906");
	UNSIGNED_TEST("907");
	UNSIGNED_TEST("908");
	UNSIGNED_TEST("909");
	UNSIGNED_TEST("910");
	UNSIGNED_TEST("911");
	UNSIGNED_TEST("912");
	UNSIGNED_TEST("913");
	UNSIGNED_TEST("914");
	UNSIGNED_TEST("915");
	UNSIGNED_TEST("916");
	UNSIGNED_TEST("917");
	UNSIGNED_TEST("918");
	UNSIGNED_TEST("919");
	UNSIGNED_TEST("920");
	UNSIGNED_TEST("921");
	UNSIGNED_TEST("922");
	UNSIGNED_TEST("923");
	UNSIGNED_TEST("924");
	UNSIGNED_TEST("925");
	UNSIGNED_TEST("926");
	UNSIGNED_TEST("927");
	UNSIGNED_TEST("928");
	UNSIGNED_TEST("929");
	UNSIGNED_TEST("930");
	UNSIGNED_TEST("931");
	UNSIGNED_TEST("932");
	UNSIGNED_TEST("933");
	UNSIGNED_TEST("934");
	UNSIGNED_TEST("935");
	UNSIGNED_TEST("936");
	UNSIGNED_TEST("937");
	UNSIGNED_TEST("938");
	UNSIGNED_TEST("939");
	UNSIGNED_TEST("940");
	UNSIGNED_TEST("941");
	UNSIGNED_TEST("942");
	UNSIGNED_TEST("943");
	UNSIGNED_TEST("944");
	UNSIGNED_TEST("945");
	UNSIGNED_TEST("946");
	UNSIGNED_TEST("947");
	UNSIGNED_TEST("948");
	UNSIGNED_TEST("949");
	UNSIGNED_TEST("950");
	UNSIGNED_TEST("951");
			 UNSIGNED_TEST("952");
			 UNSIGNED_TEST("953");
			 UNSIGNED_TEST("954");
			 UNSIGNED_TEST("955");
			 UNSIGNED_TEST("956");
			 UNSIGNED_TEST("957");
			 UNSIGNED_TEST("958");
			 UNSIGNED_TEST("959");
			 UNSIGNED_TEST("960");
			 UNSIGNED_TEST("961");
			 UNSIGNED_TEST("962");
			 UNSIGNED_TEST("963");
			 UNSIGNED_TEST("964");
			 UNSIGNED_TEST("965");
			 UNSIGNED_TEST("966");
			 UNSIGNED_TEST("967");
			 UNSIGNED_TEST("968");
			 UNSIGNED_TEST("969");
			 UNSIGNED_TEST("970");
			 UNSIGNED_TEST("971");
			 UNSIGNED_TEST("972");
			 UNSIGNED_TEST("973");
			 UNSIGNED_TEST("974");
			 UNSIGNED_TEST("975");
			 UNSIGNED_TEST("976");
			 UNSIGNED_TEST("977");
			 UNSIGNED_TEST("978");
			 UNSIGNED_TEST("979");
			 UNSIGNED_TEST("980");
			 UNSIGNED_TEST("981");
			 UNSIGNED_TEST("982");
			 UNSIGNED_TEST("983");
			 UNSIGNED_TEST("984");
			 UNSIGNED_TEST("985");
			 UNSIGNED_TEST("986");
			 UNSIGNED_TEST("987");
			 UNSIGNED_TEST("988");
			 UNSIGNED_TEST("989");
			 UNSIGNED_TEST("990");
			 UNSIGNED_TEST("991");
			 UNSIGNED_TEST("992");
			 UNSIGNED_TEST("993");
			 UNSIGNED_TEST("994");
			 UNSIGNED_TEST("995");
			 UNSIGNED_TEST("996");
			 UNSIGNED_TEST("997");
			 UNSIGNED_TEST("998");
			 UNSIGNED_TEST("999");
			 UNSIGNED_TEST("1000");
			 UNSIGNED_TEST("1001");
			 UNSIGNED_TEST("1002");
			 UNSIGNED_TEST("1003");
			 UNSIGNED_TEST("1004");
			 UNSIGNED_TEST("1005");
			 UNSIGNED_TEST("1006");
			 UNSIGNED_TEST("1007");
			 UNSIGNED_TEST("1008");
			 UNSIGNED_TEST("1009");
			 UNSIGNED_TEST("1010");
			 UNSIGNED_TEST("1011");
			 UNSIGNED_TEST("1012");
			 UNSIGNED_TEST("1013");
			 UNSIGNED_TEST("1014");
			 UNSIGNED_TEST("1015");
			 UNSIGNED_TEST("1016");
			 UNSIGNED_TEST("1017");
			 UNSIGNED_TEST("1018");
			 UNSIGNED_TEST("1019");
			 UNSIGNED_TEST("1020");
			 UNSIGNED_TEST("1021");
			 UNSIGNED_TEST("1022");
			 UNSIGNED_TEST("1023");
			 UNSIGNED_TEST("1024");
}

void	test_lu_convert(void) {
# define LONG_UNSIGNED_TEST(s_nbr) do {					\
		unsigned long _unb = 0;					\
		unsigned long _expected = (unsigned long)strtoul(s_nbr, NULL, 10); \
									\
		test(s_nbr, "%lu", &_unb);				\
		if (_unb != _expected) {				\
		dprintf(2, "expected: %lu, got: %lu\n", _expected, _unb); \
			}						\
		assert(_unb == _expected);				\
		} while (0);

	size_t	i = 0;

# define  NBR_TESTS 10000

	while (i < NBR_TESTS) {
		char	s_nbr[256];

		snprintf(s_nbr, sizeof(s_nbr), "%lu", (unsigned long)i);
		LONG_UNSIGNED_TEST(s_nbr);
		i++;
	}

	i = 0;
	// Test whitespaces
	while (i < NBR_TESTS) {
		char	s_nbr[256];

		snprintf(s_nbr, sizeof(s_nbr), "       %lu           ", (unsigned long)i);
		LONG_UNSIGNED_TEST(s_nbr);
		i++;
	}

	i = 0;
	// Test multiple nbrs
	while (i < NBR_TESTS) {
		char	s_nbr[256];
		unsigned long	expected_1 = (unsigned long)i;
		unsigned long	expected_2 = (unsigned long)i * 2;
		unsigned long	nbr_1 = 0;
		unsigned long	nbr_2 = 0;


		snprintf(s_nbr, sizeof(s_nbr), "   %lu    %lu  ", expected_1, expected_2);
		test(s_nbr, "%lu %lu", &nbr_1, &nbr_2);
		assert(nbr_1 == expected_1);
		assert(nbr_2 == expected_2);
		i++;

	}

}
