#! /usr/bin/python
import os
import datetime

keys = []
now = datetime.datetime.now()
header = r'''# SOME DESCRIPTIVE TITLE.
# Copyright (C) YEAR THE PACKAGE'S COPYRIGHT HOLDER
# This file is distributed under the same license as the PACKAGE package.
# FIRST AUTHOR <EMAIL@ADDRESS>, YEAR.
#
msgid ""
msgstr ""
"Project-Id-Version: PACKAGE VERSION\n"
"Report-Msgid-Bugs-To: \n"
"POT-Creation-Date: {now}\n"
"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\n"
"Last-Translator: FULL NAME <EMAIL@ADDRESS>\n"
"Language-Team: LANGUAGE <LL@li.org>\n"
"Language: \n"
"MIME-Version: 1.0\n"
"Content-Type: text/plain; charset=UTF-8\n"
"Content-Transfer-Encoding: 8bit\n"

'''

# this extracts translatable strings from the \en\*.text.hs Texts files
def extract(filename):
    if os.path.exists(filename):
        infile = open(filename, 'r')
        for line in infile.readlines():
            keys.append(line.strip('\n').replace("\"", "\\\""))

# this removes duplicates without screwing up the order
def remove_duplicates():
    global keys
    tmp = []
    for node in keys:
        if node not in tmp:
            tmp.append(node)
    keys = tmp

# this writes the list of keys to a gettext .po file
def save(filename):
    outfile = open('data.pot', 'w')
    outfile.write(header.format(now=now.strftime('%Y-%m-%d %H:%M+%z')))
    remove_duplicates()
    for line in keys:
        outfile.write("msgid \"{line}\"\n".format(line=line))
        outfile.write("msgstr \"\"\n\n")

# HERE'S THE MAIN EXECUTION
extract('en/Benedictions.text.hs')
extract('en/Dialogs.text.hs')
extract('en/Docs.text.hs')
extract('en/Entities.text.hs')
extract('en/Items.text.hs')
extract('en/Maps.text.hs')
extract('en/Menus.text.hs')
extract('en/Miracles.text.hs')
extract('en/Quests.text.hs')

save('data.pot')
