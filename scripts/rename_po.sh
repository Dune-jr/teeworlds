#!/bin/bash

# to execute in datasrc/languages, with a teeworlds_teeworlds_teeworlds.zip file in parameter
# use: ./rename_po.sh teeworlds_teeworlds_teeworlds.zip

## Index
# ar	Arabic 
# ar_AA	Arabic (Unitag)
# az	Azerbaijani 
# be	Belarusian 
# bg	Bulgarian 
# br	Breton 
# bs	Bosnian 
# ca	Catalan 
# cs	Czech 
# cv	Chuvash 
# da	Danish 
# de	German 
# el	Greek 
# en_GB	English (United Kingdom)
# eo	Esperanto 
# es	Spanish 
# et	Estonian 
# fa	Persian 
# fi	Finnish 
# fr	French 
# ga	Irish 
# gd	Gaelic, Scottish 
# gl_ES	Galician (Spain) 
# hu	Hungarian 
# id	Indonesia
# it	Italian 
# ja	Japanese 
# jbo	Lojban
# kk	Kazakh
# ko	Korean 
# ky	Kyrgyz 
# lt	Lithuanian 
# mn	Mongolian
# nl	Dutch 
# no	Norwegian 
# oc	Occitan (post 1500)
# pl	Polish 
# pt	Portuguese 
# pt_BR	Portuguese (Brazil) 
# ro	Romanian 
# ru	Russian 
# sah	Sakha (Yakut) 
# sl	Slovenian
# sq	Albanian
# sr	Serbian 
# sv	Swedish 
# tk	Turkmen
# tr	Turkish 
# tt	Tatar
# uk	Ukrainian 
# uz	Uzbek
# zh	Chinese 
# zh_HK	Chinese (Hong Kong) 
# zh_TW	Chinese (Taiwan) 

FILE=$1
if [ -z $FILE ]; then
	echo "Please provide a teeworlds.zip archive of .po files"
elif [ -f $FILE ]; then
	echo "Unzipping $FILE"
	unzip $FILE

	declare -a po_files=(
		"teeworlds_ar.po"
		"teeworlds_ar_AA.po"
		"teeworlds_az.po"
		"teeworlds_be.po"
		"teeworlds_bg.po"
		"teeworlds_br.po"
		"teeworlds_bs.po"
		"teeworlds_ca.po"
		"teeworlds_cs.po"
		"teeworlds_cv.po"
		"teeworlds_da.po"
		"teeworlds_de.po"
		"teeworlds_el.po"
		"teeworlds_en.po"
		"teeworlds_en_GB.po"
		"teeworlds_eo.po"
		"teeworlds_es.po"
		"teeworlds_et.po"
		"teeworlds_fa.po"
		"teeworlds_fi.po"
		"teeworlds_fr.po"
		"teeworlds_ga.po"
		"teeworlds_gd.po"
		"teeworlds_gl_ES.po"
		"teeworlds_hu.po"
		"teeworlds_id.po"
		"teeworlds_it.po"
		"teeworlds_ja.po"
		"teeworlds_jbo.po"
		"teeworlds_kk.po"
		"teeworlds_ko.po"
		"teeworlds_ky.po"
		"teeworlds_lt.po"
		"teeworlds_mn.po"
		"teeworlds_nl.po"
		"teeworlds_no.po"
		"teeworlds_oc.po"
		"teeworlds_pl.po"
		"teeworlds_pt.po"
		"teeworlds_pt_BR.po"
		"teeworlds_ro.po"
		"teeworlds_ru.po"
		"teeworlds_sah.po"
		"teeworlds_sl.po"
		"teeworlds_sq.po"
		"teeworlds_sr.po"
		"teeworlds_sv.po"
		"teeworlds_tk.po"
		"teeworlds_tr.po"
		"teeworlds_tt.po"
		"teeworlds_uk.po"
		"teeworlds_uz.po"
		"teeworlds_zh.po"
		"teeworlds_zh_HK.po"
		"teeworlds_zh_TW.po"
	)

	declare -a useless_po_files=(
		"teeworlds_ar.po"
		"teeworlds_ar_AA.po"
		"teeworlds_az.po"
		"teeworlds_en.po"
		"teeworlds_en_GB.po"
		"teeworlds_id.po"
		"teeworlds_fa.po"
		"teeworlds_jbo.po"
		"teeworlds_kk.po"
		"teeworlds_mn.po"
		"teeworlds_oc.po"
		"teeworlds_sah.po"
		"teeworlds_sq.po"
		"teeworlds_tk.po"
		"teeworlds_tt.po"
		"teeworlds_uz.po"
		"teeworlds_zh_HK.po"
	)

	mv -v teeworlds_be.po	belarusian.po
	mv -v teeworlds_bg.po	bulgarian.po
	mv -v teeworlds_br.po	breton.po
	mv -v teeworlds_bs.po	bosnian.po
	mv -v teeworlds_ca.po	catalan.po
	mv -v teeworlds_cs.po	czech.po
	mv -v teeworlds_cv.po	chuvash.po
	mv -v teeworlds_da.po	danish.po
	mv -v teeworlds_de.po	german.po
	mv -v teeworlds_el.po	greek.po
	mv -v teeworlds_eo.po	esperanto.po
	mv -v teeworlds_es.po	spanish.po
	mv -v teeworlds_et.po	estonian.po
	mv -v teeworlds_fi.po	finnish.po
	mv -v teeworlds_fr.po	french.po
	mv -v teeworlds_ga.po	irish.po
	mv -v teeworlds_gd.po	gaelic_scottish.po
	mv -v teeworlds_gl_ES.po	galician.po
	mv -v teeworlds_hu.po	hungarian.po
	mv -v teeworlds_it.po	italian.po
	mv -v teeworlds_ja.po	japanese.po
	mv -v teeworlds_ko.po	korean.po
	mv -v teeworlds_ky.po	kyrgyz.po
	mv -v teeworlds_lt.po	lithuanian.po
	mv -v teeworlds_nl.po	dutch.po
	mv -v teeworlds_no.po	norwegian.po
	mv -v teeworlds_pl.po	polish.po
	mv -v teeworlds_pt.po	portuguese.po
	mv -v teeworlds_pt_BR.po	brazilian_portuguese.po
	mv -v teeworlds_ro.po	romanian.po
	mv -v teeworlds_ru.po	russian.po
	mv -v teeworlds_sl.po	slovenian.po
	mv -v teeworlds_sr.po	serbian.po
	mv -v teeworlds_sv.po	swedish.po
	mv -v teeworlds_tr.po	turkish.po
	mv -v teeworlds_uk.po	ukrainian.po
	mv -v teeworlds_zh.po	simplified_chinese.po
	mv -v teeworlds_zh_TW.po	traditional_chinese.po

	for i in "${useless_po_files[@]}"
	do
		rm -v "$i"
	done

else
	echo "Archive file $FILE does not exist."
fi