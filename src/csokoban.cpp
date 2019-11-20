#include "csokoban.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "crelateddirs.h"
#include "clevelset.h"
#include "cplayboard.h"
#include "cpixmaps.h"

CSokoban* CSokoban::sokoban_;

CSokoban::CSokoban(int& argc, char** argv)
  : QApplication(argc, argv)
{
  commandline_ = argv[0];
  if (!sokoban_) sokoban_ = this;
  init();
}

// ------- CSokoban::init ---------
void CSokoban::init()
{
#define FUNCTION "CSokoban::init"
  // check
  QString path_home(CRelatedDirs::get_dir(CRelatedDirs::DI_home));
  QString path_share(CRelatedDirs::get_dir(CRelatedDirs::DI_share));
  QString path_here;

  QFileInfo info(commandline_);
  path_here = info.absolutePath();
  path_here.replace(QChar('/'), QDir::separator());
  path_here += QDir::separator();

  QString path_here_parent(path_here);
  path_here_parent += "..";
  path_here_parent += QDir::separator();

  qDebug() << FUNCTION ": at [1.1] path_home=" << path_home;
  qDebug() << FUNCTION ": at [1.2] path_share=" << path_share;
  qDebug() << FUNCTION ": at [1.3] path_here=" << path_here;
  qDebug() << FUNCTION ": at [1.4] path_here_parent=" << path_here_parent;
  QStringList ps;

  // search in /usr/share/soko4/levels, ${HOME}/.soko4/levels, ../levels
  {
    QString s_levels(path_share);
    s_levels += "levels";
    QDir dir2(s_levels);
    if (dir2.exists()) {
      ps.push_back(s_levels); // todo: use app path
    }
  }
  {
    QString s_levels(path_home);
    s_levels += "levels";
    QDir dir2(s_levels);
    if (dir2.exists()) {
      ps.push_back(s_levels); // todo: use app path
    }
  }
  {
    QString s_levels(path_here_parent);
    s_levels += "levels";
    QDir dir2(s_levels);
    if (dir2.exists()) {
      ps.push_back(s_levels); // todo: use app path
    }
  }

  {
    QString s_levels("/app/levels");
    QDir dir2(s_levels);
    if (dir2.exists()) {
      ps.push_back(s_levels);
    }
  }

  {
    qDebug() << FUNCTION ": at [1.5] ps.size()==" << ps.size();
    QStringList::iterator it = ps.begin();
    for (; it != ps.end(); ++it) {
      qDebug() << FUNCTION ": at [1.5.1]" << *it;
    }
  }

  {
    QString s_themes(path_share);
    s_themes += "themes";
    QDir dir2(s_themes);
    if (dir2.exists()) {
      CPixmaps::instance()->load(s_themes);
    }
  }
  {
    QString s_themes(path_home);
    s_themes += "themes";
    QDir dir2(s_themes);
    if (dir2.exists()) {
      CPixmaps::instance()->load(s_themes);
    }
  }
  {
    QString s_themes(path_here_parent);
    s_themes += "themes";
    QDir dir2(s_themes);
    if (dir2.exists()) {
      CPixmaps::instance()->load(s_themes);
    }
  }

  {
    QString s_themes("/app/themes");
    QDir dir2(s_themes);
    if (dir2.exists()) {
      CPixmaps::instance()->load(s_themes);
    }
  }
  qDebug() << FUNCTION ": at [2]";
  QStringList::const_iterator xi;
  for (xi = ps.constBegin(); xi != ps.constEnd(); ++xi) {
    qDebug() << FUNCTION " at [2.0.5] " << *xi;
    QDir dir2(*xi);
    QStringList list(dir2.entryList());
    QStringList::const_iterator xj;
    for (xj = list.constBegin(); xj != list.constEnd(); ++xj) {
      if ( (*xj).startsWith("."))
        continue;
      qDebug() << FUNCTION " at [2.1] " << *xj;
      QString p3(*xi);
      p3 += QDir::separator();
      p3 += *xj;
      if (!levelsets_.contains(*xj)) {
        qDebug() << FUNCTION " at [2.2]";
        levelsets_.push_back(*xj);
        CLevelSet* tmp = CLevelSet::create_by_filename(p3, *xj);
      }
    }
  }
  qDebug() << FUNCTION ": at [3]";
  //qDebug() << FUNCTION " at [3] before emit setLevelSets, levelsets.size()=" << levelsets.size();
  // TODO(asartakov): skip playd levels
  last_ = path_home;
  last_ += "last";
  QFile last_file(last_);
  cur_level_num_ = -1;
  if (last_file.exists()) {
    last_file.open(QIODevice::ReadOnly);

    QString levelset_name = last_file.readLine();
    while (levelset_name.endsWith("\x0A")||levelset_name.endsWith("\x0D")) {
      levelset_name.remove(levelset_name.size() - 1, 1);
    }

    QString levelnum = last_file.readLine();

    QString sha = last_file.readLine();
    while (sha.endsWith("\x0A") || sha.endsWith("\x0D")) {
      sha.remove(sha.size() - 1, 1);
    }
    QString theme_name = last_file.readLine();
    while (theme_name.endsWith("\x0A") || theme_name.endsWith("\x0D")) {
      theme_name.remove(theme_name.size() - 1, 1);
    }
    CPixmaps::instance()->select_theme(theme_name);
    Q_EMIT setTheme(theme_name);
    last_file.close();
    if (levelset_name.size()) {
      cur_levelset_name_ = levelset_name;
      CLevelSet* tmp_set = CLevelSet::getSet(cur_levelset_name_);
      if (tmp_set) cur_levelset_name_ = tmp_set->getName();
      cur_level_num_ = levelnum.toInt();
      if (cur_levelset_name_ != levelset_name) cur_level_num_ = -1;
      if (cur_level_num_ >= 0) {
        // sha
        CPlayBoard::instance()->reset(*getCurLevel());
        if (CPlayBoard::instance()->get_signature() != sha) {
          qDebug() << FUNCTION " at [4] sha=" << sha << "get_signature()=" << CPlayBoard::instance()->get_signature();
          cur_level_num_ = 0;
        }
      } else {
        qDebug() << FUNCTION " at [5]" ;
      }
    }
  }
  qDebug() << FUNCTION ": at [6]";
  if (cur_level_num_ < 0) {
    cur_level_num_ = 0;
  }
  qDebug() << FUNCTION ": at [7]";
  Q_EMIT setLevelset(cur_levelset_name_);
  // set list of levelset in main menu
  qDebug() << FUNCTION ": at [8]";
  CLevel* cur_level = getCurLevel();
  if (cur_level) {
    CPlayBoard::instance()->reset(*cur_level);
  } else {
    qDebug() << FUNCTION ": at [8.1] no cur level, abort";
    abort();
  }
  qDebug() << FUNCTION ": at [9]";
  levelInfo();
  qDebug() << FUNCTION ": at [10]";
  repaint();
  qDebug() << FUNCTION ": at [11]";
  text_window_ = NULL;
#undef FUNCTION
}

void CSokoban::saveLast()
{
#define FUNCTION "CSokoban::saveLast"
  qDebug() << FUNCTION " at [0]" ;
  QFile file(last_);
  file.open(QIODevice::WriteOnly);
  QString line = cur_levelset_name_ + "\x0A";
  file.write(line.toUtf8());
  line.sprintf("%d\n",cur_level_num_);
  file.write(line.toUtf8());
  // sha
  line = CPlayBoard::instance()->get_signature()+"\x0A";
  file.write(line.toUtf8());
  // theme
  QString theme(CPixmaps::instance()->get_cur_theme_name());
  theme += "\x0A";
  file.write(theme.toUtf8());
  file.close();
#undef FUNCTION
}

void CSokoban::levelInfo()
{
  CLevelSet* ls = CLevelSet::getSet(cur_levelset_name_);
  CLevel* lvl = ls->getLevel(cur_level_num_);
  QString cur_level_name = lvl->getName();
  int nn = ls->get_num_of_levels();
  Q_EMIT levelParam(cur_levelset_name_, nn, cur_level_num_, cur_level_name);
}

// --------- CSokoban::getCurLevel ----------
CLevel* CSokoban::getCurLevel()
{
#define FUNCTION "CSokoban::getCurLevel"
  CLevelSet* cur_ls = CLevelSet::getSet(cur_levelset_name_);
  if (!cur_ls) {
    qDebug() << FUNCTION " at [1] return NULL. cur_levelset_name_=='" << cur_levelset_name_ <<"'";
    return NULL;
  }
  qDebug() << FUNCTION " at [2]" ;
  cur_levelset_name_ = cur_ls->getName();
  qDebug() << FUNCTION " at [3]" ;
  return cur_ls->getLevel(cur_level_num_);
#undef FUNCTION
}

// ---------- CSokoban::goToTheNextLevel --------
void CSokoban::goToTheNextLevel()
{
#define FUNCTION "CSokoban::goToTheNextLevel"
  qDebug() << FUNCTION " at [0]";
  CLevelSet* cur_ls = CLevelSet::getSet(cur_levelset_name_);
  if (!cur_ls) {
    qDebug() << FUNCTION " return at [1] ";
    return;
  }
  CLevel* lvl = cur_ls->getLevel(cur_level_num_ + 1);
  if (lvl) {
    ++cur_level_num_;
    qDebug() << FUNCTION " at [2] cur_level_num_=" << cur_level_num_;
    CPlayBoard::instance()->reset(*getCurLevel());
    levelInfo();
    Q_EMIT repaint();
  }
  qDebug() << FUNCTION " return at [3] cur_level_num_=" << cur_level_num_;
  return;
#undef FUNCTION
}

// ---------- CSokoban::goToTheNextULevel --------
void CSokoban::goToTheNextULevel()
{
#define FUNCTION "CSokoban::goToTheNextULevel"
  int cur_level_num = cur_level_num_;
  qDebug() << FUNCTION " at [1] cur_level_num == " << cur_level_num;
  for (;;) {
    goToTheNextLevel();
    if ( (cur_level_num == cur_level_num_) ) {
      qDebug() << FUNCTION " at [2] break cur_level_num == " << cur_level_num
               << " cur_level_num_ == " << cur_level_num_;
      break;
    }
    if (!CPlayBoard::instance()->does_solution_exist()) {
      qDebug() << FUNCTION " at [3] break";
      break;
    }
    cur_level_num = cur_level_num_;
  }
#undef FUNCTION
}

// --------- CSokoban::goToThePrevLevel --------
void CSokoban::goToThePrevLevel()
{
#define FUNCTION "CSokoban::goToThePrevLevel"
  qDebug() << FUNCTION " at [0]";
  if (cur_level_num_) {
    --cur_level_num_;
    //CLevel * lvl=cur_ls->getLevel(cur_level_num_);
    qDebug() << FUNCTION " return at [2] cur_level_num_=" << cur_level_num_;
    CPlayBoard::instance()->reset(*getCurLevel());
    levelInfo();
    emit repaint();
  }
#undef FUNCTION
}

// --------- CSokoban::goToThePrevULevel --------
void CSokoban::goToThePrevULevel()
{
#define FUNCTION "CSokoban::goToThePrevULevel"
  qDebug() << FUNCTION " at [0]";
  int cur_level_num = cur_level_num_;
  for (;;) {
    goToThePrevLevel();
    if ( cur_level_num == cur_level_num_) break;
    if (!CPlayBoard::instance()->does_solution_exist()) break;
  }
#undef FUNCTION
}

// ---------- CSokoban::showTextWindow -----------
void CSokoban::showTextWindow()
{
#define FUNCTION "CSokoban::showTextWindow"
  qDebug() << FUNCTION " at [0]";
  if (text_window_) {
    return;
  }
  qDebug() << FUNCTION " at [1]";
  text_window_ = new CTextWindow(NULL);
  QObject::connect(text_window_, SIGNAL(closing()), this, SLOT(handleClosingTextWindow()));
  QObject::connect(text_window_, SIGNAL(repaint()), this, SIGNAL(repaint()));
#undef FUNCTION
}

// -------- CSokoban::handleClosingTextWindow ------
void CSokoban::handleClosingTextWindow()
{
#define FUNCTION "CSokoban::handleClosingTextWindow"
  qDebug() << FUNCTION " at [0]";
  text_window_ = NULL;
#undef FUNCTION
}

// ------- CSokoban::repeatSignals -----
void CSokoban::repeatSignals()
{
  levelInfo();
  emit setLevelSets(levelsets_);
  emit setThemes();
  emit setLevelset(cur_levelset_name_);
  emit setTheme(CPixmaps::instance()->get_cur_theme_name());
}

void CSokoban::handleSetLevelSet(const QString& name)
{
#define FUNCTION "CSokoban::handleSetLevelSet"
  qDebug() << FUNCTION " at [0] name='" << name << "'";
  cur_levelset_name_ = name;
  cur_level_num_ = 0;
  CPlayBoard::instance()->reset(*getCurLevel());
  levelInfo();
  Q_EMIT repaint();
#undef FUNCTION
}

void CSokoban::handleLevel1()
{
#define FUNCTION "CSokoban::handleLevel1"
  qDebug() << FUNCTION " at [0]";
  cur_level_num_ = 0;
  CPlayBoard::instance()->reset(*getCurLevel());
  levelInfo();
  emit repaint();
#undef FUNCTION
}
