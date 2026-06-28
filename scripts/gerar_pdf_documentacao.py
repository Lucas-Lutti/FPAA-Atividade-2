from pathlib import Path
import re

from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import cm
from reportlab.platypus import (
    Image,
    Paragraph,
    Preformatted,
    SimpleDocTemplate,
    Spacer,
    Table,
    TableStyle,
)


RAIZ = Path(__file__).resolve().parents[1]
ARQUIVO_MD = RAIZ / "docs" / "documentacao.md"
SAIDA_PRINCIPAL = RAIZ / "output" / "pdf" / "documentacao_tango.pdf"
SAIDA_DOCS = RAIZ / "docs" / "documentacao.pdf"


def limpar_texto(texto):
    texto = texto.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
    texto = re.sub(r"`([^`]+)`", r"<font name='Courier'>\1</font>", texto)
    texto = texto.replace("**", "")
    return texto


def celula_tabela(texto):
    texto = texto.strip()
    imagem = re.search(r'<img src="([^"]+)" width="([^"]+)">', texto)
    if imagem:
        caminho_imagem = (ARQUIVO_MD.parent / imagem.group(1)).resolve()
        resto = texto[imagem.end():].strip()
        return [
            Image(str(caminho_imagem), width=0.55 * cm, height=0.55 * cm),
            Paragraph(limpar_texto(resto), ESTILOS["Normal"]),
        ]
    return Paragraph(limpar_texto(texto), ESTILOS["Normal"])


def tabela_markdown(linhas):
    dados = []
    for indice, linha in enumerate(linhas):
        if indice == 1 and set(linha.replace("|", "").replace("-", "").replace(" ", "")) == set():
            continue
        partes = [parte.strip() for parte in linha.strip().strip("|").split("|")]
        dados.append([celula_tabela(parte) for parte in partes])

    tabela = Table(dados, hAlign="LEFT", colWidths=[6.0 * cm, 6.0 * cm, 3.0 * cm])
    tabela.setStyle(
        TableStyle(
            [
                ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#EAEFF7")),
                ("TEXTCOLOR", (0, 0), (-1, 0), colors.HexColor("#111827")),
                ("GRID", (0, 0), (-1, -1), 0.35, colors.HexColor("#CBD5E1")),
                ("VALIGN", (0, 0), (-1, -1), "MIDDLE"),
                ("LEFTPADDING", (0, 0), (-1, -1), 7),
                ("RIGHTPADDING", (0, 0), (-1, -1), 7),
                ("TOPPADDING", (0, 0), (-1, -1), 6),
                ("BOTTOMPADDING", (0, 0), (-1, -1), 6),
            ]
        )
    )
    return tabela


def bloco_codigo(linhas):
    texto = "\n".join(linhas)
    pre = Preformatted(texto, ESTILOS["Codigo"])
    tabela = Table([[pre]], colWidths=[16.3 * cm], hAlign="LEFT")
    tabela.setStyle(
        TableStyle(
            [
                ("BACKGROUND", (0, 0), (-1, -1), colors.HexColor("#F3F4F6")),
                ("BOX", (0, 0), (-1, -1), 0.25, colors.HexColor("#D1D5DB")),
                ("LEFTPADDING", (0, 0), (-1, -1), 7),
                ("RIGHTPADDING", (0, 0), (-1, -1), 7),
                ("TOPPADDING", (0, 0), (-1, -1), 6),
                ("BOTTOMPADDING", (0, 0), (-1, -1), 6),
            ]
        )
    )
    return tabela


def rodape(canvas, documento):
    canvas.saveState()
    canvas.setFont("Helvetica", 8)
    canvas.setFillColor(colors.HexColor("#6B7280"))
    canvas.drawRightString(19.0 * cm, 1.2 * cm, f"Pagina {documento.page}")
    canvas.restoreState()


def montar_elementos(markdown):
    elementos = []
    linhas = markdown.splitlines()
    indice = 0

    while indice < len(linhas):
        linha = linhas[indice]
        texto = linha.strip()

        if not texto:
            elementos.append(Spacer(1, 0.15 * cm))
            indice += 1
            continue

        if texto.startswith("```"):
            indice += 1
            codigo = []
            while indice < len(linhas) and not linhas[indice].strip().startswith("```"):
                codigo.append(linhas[indice])
                indice += 1
            elementos.append(bloco_codigo(codigo))
            elementos.append(Spacer(1, 0.25 * cm))
            indice += 1
            continue

        if texto.startswith("|"):
            tabela_linhas = []
            while indice < len(linhas) and linhas[indice].strip().startswith("|"):
                tabela_linhas.append(linhas[indice])
                indice += 1
            elementos.append(tabela_markdown(tabela_linhas))
            elementos.append(Spacer(1, 0.25 * cm))
            continue

        if texto.startswith("# "):
            elementos.append(Paragraph(limpar_texto(texto[2:]), ESTILOS["Titulo"]))
            elementos.append(Spacer(1, 0.35 * cm))
        elif texto.startswith("## "):
            elementos.append(Paragraph(limpar_texto(texto[3:]), ESTILOS["Secao"]))
            elementos.append(Spacer(1, 0.15 * cm))
        elif texto.startswith("- "):
            elementos.append(Paragraph("- " + limpar_texto(texto[2:]), ESTILOS["Normal"]))
        else:
            elementos.append(Paragraph(limpar_texto(texto), ESTILOS["Normal"]))

        indice += 1

    return elementos


ESTILOS_BASE = getSampleStyleSheet()
ESTILOS = {
    "Titulo": ParagraphStyle(
        "Titulo",
        parent=ESTILOS_BASE["Title"],
        fontName="Helvetica-Bold",
        fontSize=20,
        leading=24,
        textColor=colors.HexColor("#111827"),
        spaceAfter=12,
    ),
    "Secao": ParagraphStyle(
        "Secao",
        parent=ESTILOS_BASE["Heading2"],
        fontName="Helvetica-Bold",
        fontSize=13,
        leading=16,
        textColor=colors.HexColor("#1F2937"),
        spaceBefore=10,
        spaceAfter=4,
    ),
    "Normal": ParagraphStyle(
        "Normal",
        parent=ESTILOS_BASE["BodyText"],
        fontName="Helvetica",
        fontSize=10.2,
        leading=14,
        textColor=colors.HexColor("#111827"),
        spaceAfter=5,
    ),
    "Codigo": ParagraphStyle(
        "Codigo",
        parent=ESTILOS_BASE["Code"],
        fontName="Courier",
        fontSize=8.0,
        leading=9.4,
        textColor=colors.HexColor("#111827"),
    ),
}


def gerar_pdf():
    SAIDA_PRINCIPAL.parent.mkdir(parents=True, exist_ok=True)
    SAIDA_DOCS.parent.mkdir(parents=True, exist_ok=True)

    for destino in (SAIDA_PRINCIPAL, SAIDA_DOCS):
        markdown = ARQUIVO_MD.read_text(encoding="utf-8")
        elementos = montar_elementos(markdown)
        documento = SimpleDocTemplate(
            str(destino),
            pagesize=A4,
            rightMargin=1.6 * cm,
            leftMargin=1.6 * cm,
            topMargin=1.5 * cm,
            bottomMargin=1.8 * cm,
            title="Trabalho Pratico 2 - FPAA",
            author="PUC Minas",
        )
        documento.build(list(elementos), onFirstPage=rodape, onLaterPages=rodape)


if __name__ == "__main__":
    gerar_pdf()
