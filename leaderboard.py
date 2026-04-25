#!/usr/bin/env python3
from __future__ import annotations

import csv
import re
from typing import List, Dict, Optional

import requests
from bs4 import BeautifulSoup, Tag

BASE_URL = "https://contest.catournament.org/public"
HEADERS = {
    "User-Agent": "Mozilla/5.0 (compatible; domjudge-html-scraper/1.0)"
}


def get_page(url: str) -> str:
    r = requests.get(url, headers=HEADERS, timeout=30)
    r.raise_for_status()
    return r.text


def clean_text(x: str) -> str:
    return re.sub(r"\s+", " ", x).strip()


def looks_like_rank(text: str) -> bool:
    text = clean_text(text)
    return text.isdigit() or text == "?"


def looks_like_team_name(text: str) -> bool:
    text = clean_text(text)
    if not text:
        return False
    if text.isdigit():
        return False
    if text.lower() in {"rank", "team", "score", "time"}:
        return False
    return True


def extract_team_name(cells: List[Tag], texts: List[str]) -> str:
    # From your screenshot, structure looks like:
    # [rank?, flag, team_name, solved, penalty, ...]
    # For unranked users, rank may be empty, so team is often at index 1 or 2.
    for idx in (2, 1, 0):
        if idx < len(texts) and looks_like_team_name(texts[idx]):
            txt = texts[idx]
            # avoid picking flag-only / score-only cells
            if len(txt) >= 3 and not re.fullmatch(r"\d+", txt):
                return txt

    # fallback: search all text cells
    for txt in texts:
        if looks_like_team_name(txt) and len(txt) >= 3:
            return txt

    return ""


def parse_problem_cells(cells: List[Tag]) -> List[str]:
    out = []
    for cell in cells:
        txt = clean_text(cell.get_text(" ", strip=True))
        title = clean_text(cell.get("title", "") or "")
        cls = " ".join(cell.get("class", []))
        if title:
            out.append(f"{txt} | {title} | {cls}".strip(" |"))
        else:
            out.append(f"{txt} | {cls}".strip(" |"))
    return out


def find_scoreboard_table(soup: BeautifulSoup) -> Optional[Tag]:
    tables = soup.find_all("table")
    if not tables:
        return None

    best = None
    best_score = -1

    for table in tables:
        rows = table.find_all("tr")
        if len(rows) < 2:
            continue

        score = 0
        for row in rows[:20]:
            cells = row.find_all(["td", "th"])
            texts = [clean_text(c.get_text(" ", strip=True)) for c in cells]

            if len(cells) >= 4:
                score += 1
            if texts and looks_like_rank(texts[0]):
                score += 2
            if any("-" in t and re.search(r"[a-zA-Z]", t) for t in texts):
                score += 2

        if score > best_score:
            best_score = score
            best = table

    return best


def scrape_scoreboard(html: str) -> List[Dict]:
    soup = BeautifulSoup(html, "html.parser")
    table = find_scoreboard_table(soup)
    if table is None:
        raise RuntimeError("No suitable scoreboard table found.")

    rows_out: List[Dict] = []

    for row in table.find_all("tr"):
        cells = row.find_all("td")
        if len(cells) < 4:
            continue

        texts = [clean_text(td.get_text(" ", strip=True)) for td in cells]

        # rank may be empty for "no attempt" users
        rank = texts[0] if texts and looks_like_rank(texts[0]) else ""

        team = extract_team_name(cells, texts)

        # based on screenshot:
        # ranked row:   [rank, flag, team, solved, penalty, ...]
        # unranked row: [flag, team, solved, penalty, ...] or [empty, flag, team, solved, penalty]
        solved = ""
        penalty = ""
        problem_start_idx = None

        # find first two numeric-looking cells after the team cell
        team_idx = -1
        for i, txt in enumerate(texts):
            if txt == team:
                team_idx = i
                break

        if team_idx != -1:
            numeric_positions = []
            for i in range(team_idx + 1, len(texts)):
                t = texts[i]
                if re.fullmatch(r"\d+", t) or re.fullmatch(r"\d+:\d+", t):
                    numeric_positions.append(i)

            if len(numeric_positions) >= 1:
                solved = texts[numeric_positions[0]]
            if len(numeric_positions) >= 2:
                penalty = texts[numeric_positions[1]]
                problem_start_idx = numeric_positions[1] + 1
            elif len(numeric_positions) >= 1:
                problem_start_idx = numeric_positions[0] + 1

        if problem_start_idx is None:
            problem_start_idx = min(len(cells), 5)

        problem_cells = parse_problem_cells(cells[problem_start_idx:])

        # keep rows with team names even if rank is empty
        if not team:
            continue

        # skip obvious junk/header rows
        joined = " ".join(texts).lower()
        if "last update" in joined or joined in {"rank team score time"}:
            continue

        rows_out.append({
            "rank": rank,                 # empty string for unranked users
            "team": team,
            "solved": solved or "0",
            "penalty": penalty or "0",
            "problems": problem_cells,
            "raw_cells": texts,
        })

    # de-duplicate
    cleaned = []
    seen = set()
    for row in rows_out:
        key = (row["rank"], row["team"], row["solved"], row["penalty"])
        if key in seen:
            continue
        seen.add(key)
        cleaned.append(row)

    return cleaned


def save_csv(rows: List[Dict], filename: str = "leaderboard.csv") -> None:
    with open(filename, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["rank", "team", "solved", "penalty", "problems"])
        for row in rows:
            writer.writerow([
                row["rank"],
                row["team"],
                row["solved"],
                row["penalty"],
                " || ".join(row["problems"]),
            ])


def main() -> None:
    html = get_page(BASE_URL)
    rows = scrape_scoreboard(html)

    if not rows:
        raise SystemExit("Found page, but could not extract leaderboard rows.")

    for row in rows[:15]:
        print(row)

    solved_counts = [int(r["solved"]) for r in rows if r["solved"] > '0']
    
    print(f"\nTeams with solved problems: {len(solved_counts)}")
    print(f"Teams with no solved problems: {len(rows) - len(solved_counts)}")
    print(f"Average solved: {sum(solved_counts) / len(solved_counts):.2f}")
    save_csv(rows)
    print(f"\nSaved {len(rows)} rows to leaderboard.csv")


if __name__ == "__main__":
    main()