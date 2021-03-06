﻿/**
* @file profile.h
* @brief 定义系统中用到的数据库操作，包括配置、数据、质控等
* @ingroup common
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-07
*/
#ifndef PROFILE_H
#define PROFILE_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QVariant>
#include <QColor>

#define X_PROPERTY(type, name, defaultValue)			\
private:												\
	QVariant m_##name;									\
														\
public:													\
	type name(const type &val = defaultValue)			\
	{													\
		if (!m_##name.isValid())						\
			m_##name = value(#name, val);				\
		return m_##name.value<type>();					\
	}													\
	void set_##name(const type &val, bool save = true)\
	{													\
		m_##name = val;									\
		setValue(#name, val, save);						\
	}


#define profile (Profile::instance())


class QSqlDatabase;


class COMMON_EXPORT Profile : public QObject
{
	Q_OBJECT

	// 样本编号前缀
    X_PROPERTY(QString, statim_sample_prefix, "S")				// 正常样本编号前缀
	X_PROPERTY(QString, qc_sample_prefix,     "QC")				// 质控样本编号前缀
    X_PROPERTY(QString, cali_sample_prefix,   "CA")             // 标定样本编号前缀
    X_PROPERTY(QString, simu_sample_prefix,   "SI")             // 模拟样本编号前缀

	// 测量结果表示方法
	X_PROPERTY(QString, result_is_preparing,          tr("***.*"))	// 结果待测或正在测量
	X_PROPERTY(QString, result_can_not_be_calculated, tr("---.-"))	// 结果无法计算

	// 结果显示标志
	X_PROPERTY(QString, lower_report_limit_flag, tr("<"))			// 低于报告下限
	X_PROPERTY(QString, upper_report_limit_flag, tr(">"))			// 高于报告上限
	X_PROPERTY(QString, lower_mark_limit_flag,   trUtf8("\342\206\223"))			// ↓ 低于标记下限, 用于出报告
	X_PROPERTY(QString, upper_mark_limit_flag,   trUtf8("\342\206\221"))			// ↑ 高于标记上限, 用于出报告
	X_PROPERTY(QString, manual_input_flag,		 tr("M"))			// 用户输入结果

	// 结果显示颜色
	X_PROPERTY(QColor, color_of_waiting,     Qt::black)			// 待测
    X_PROPERTY(QColor, color_of_reading,     Qt::darkYellow)	// 读取数据
	X_PROPERTY(QColor, color_of_analysing,   Qt::darkBlue)		// 正在分析
	X_PROPERTY(QColor, color_of_null_result, Qt::red)			// 无测量结果

	// 测量界面
    X_PROPERTY(QString, analysis_display_assay, tr("序号;车型;车号;重量;速度;日期"))	// 默认测量界面显示项目

	// 质控
	X_PROPERTY(int, max_qc_file, 10)							// 最大质控文件
    X_PROPERTY(int, qc_method, 0)								// 质控方法

	// 通用信息
    X_PROPERTY(QString, corporation, tr("Corporation Name"))			// 公司名称

	// 系统配置
	X_PROPERTY(bool, is_touch_screen, false)					// 触摸屏

public:
	explicit Profile(QObject *parent = 0);
	Profile(const QString &filename, QObject *parent = 0);
	~Profile();

	QSqlDatabase database() const;
	QSqlDatabase qcDatabase() const;
	QSqlDatabase dataDatabase() const;

	void setValue(const QString &key, const QVariant &value, bool save = true);
	QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    template<class T> T value(const QString &key) { return value(key).value<T>(); }
    // 如果一个键没有对应的值, 用另一个缺省值代替(例如全局值)
    QVariant value2(const QString &key, const QString &defaultKey, const QVariant &defaultValue = QVariant());

	void remove(const QString &key);
	QStringList keys() const;

	static Profile &instance();

signals:
	void valueChanged(const QString &key, const QVariant &value);
	void keyRemoved(const QString &key);

private slots:
	void saveValue(const QString &key, const QVariant &value);
	void removeKey(const QString &key);

private:

	struct Data;
	Data *d;

	void init(const QString &filename);
};

#endif // PROFILE_H
