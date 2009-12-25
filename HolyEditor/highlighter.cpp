 #include <QtGui>

 #include "highlighter.h"



 Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent)
 {
     HighlightingRule rule;

     keywordFormat.setForeground(Qt::darkBlue);
     keywordFormat.setFontWeight(QFont::Bold);
     QStringList keywordPatterns;
     keywordPatterns << "\\bif\\b" << "\\bmain\\b" << "\\bend\\b" << "\\belse\\b"  << "\\bthen\\b"  ;
     foreach (QString pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
         rule.format = keywordFormat;
         highlightingRules.append(rule);
     }

     classFormat.setFontWeight(QFont::Bold);
     classFormat.setForeground(Qt::darkMagenta);
     rule.pattern = QRegExp("[0-9]");
     rule.format = classFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::blue);
     rule.pattern = QRegExp("[*]");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     multiLineCommentFormat.setForeground(Qt::red);

     quotationFormat.setForeground(Qt::darkGreen);
     rule.pattern = QRegExp("\".*\"");
     rule.format = quotationFormat;
     highlightingRules.append(rule);

     functionFormat.setForeground(Qt::black);
     functionFormat.setFontWeight(QFont::Bold);
     QStringList keywordPatterns2;
     keywordPatterns2
                     << "\\brand\\b"
                     << "\\bno_speak\\b"
                     << "\\bquest\\b"
                     << "\\bentity_dead\\b"
                     << "\\bvariable\\b"
                     << "\\bincrementeVariable\\b"
                     << "\\bspeak\\b"
                     << "\\bnewQuest\\b"
                     << "\\bsetQuestName\\b"
                     << "\\bsetQuestState\\b"
                     << "\\bsetQuestPosition\\b"
                     << "\\bnoTalk\\b"
                     << "\\bentity_variable\\b"
                     << "\\bsetTile\\b"
                     << "\\baddCash\\b"
                     << "\\bexist_item\\b" ;
     foreach (QString pattern, keywordPatterns2) {
         rule.pattern = QRegExp(pattern);
         rule.format = functionFormat;
         highlightingRules.append(rule);
     }

     commentStartExpression = QRegExp("*");
     commentEndExpression = QRegExp("*");
 }

 void Highlighter::highlightBlock(const QString &text)
 {
     foreach (HighlightingRule rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = text.indexOf(expression);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = text.indexOf(expression, index + length);
         }
     }

      setCurrentBlockState(0);

    int startIndex = 0;
     if (previousBlockState() != 1)
         startIndex = text.indexOf(commentStartExpression);

     while (startIndex >= 0) {
         int endIndex = text.indexOf(commentEndExpression, startIndex);
         int commentLength;
         if (endIndex == -1) {
             setCurrentBlockState(1);
             commentLength = text.length() - startIndex;
         } else {
             commentLength = endIndex - startIndex
                             + commentEndExpression.matchedLength();
         }
         setFormat(startIndex, commentLength, multiLineCommentFormat);
         startIndex = text.indexOf(commentStartExpression,
                                                 startIndex + commentLength);
     }





 }
