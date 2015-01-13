#include "stdafx.h"
#include "LxComposeDoc.h"
#include "LxCursor.h"
#include "GD_proxy_base.h"

#ifndef __LX_RENDER_H
#define __LX_RENDER_H

/*
��render�л����ĵ���ҳ���Ρ���
����ҳ����
��ҳ�Ļ��ƺͲ��ֻ����ڲ�ͬ������ʵ�֣�ͬ�����Ű�Ͳ����Ű�һ��
*/
class LxRender
{
public:
	LxRender() = default;
	virtual ~LxRender() = default;
public:
	virtual void create_caret(int height, int width) = 0;
	virtual void show_caret(LxCursor* cursor) = 0;
	virtual void hide_caret() = 0;
	virtual void DrawDocument(CDC* pDC) = 0;
	virtual ComposeDoc* get_compose_doc() = 0;
	virtual GD_proxy_base* get_gd_proxy() = 0;
};

class LxContexRender : public LxRender
{
public:
	LxContexRender() = delete;
	LxContexRender(const LxContexRender&) = delete;
	LxContexRender(ComposeDoc* compose_doc, GD_proxy_base* gd_proxy);
	virtual ~LxContexRender();
public:
	virtual void create_caret(int height, int width) override;
	virtual void show_caret(LxCursor* cursor) override;
	virtual void hide_caret() override;
	virtual void DrawDocument(CDC* pDC) override;
	virtual ComposeDoc* get_compose_doc() override;
	virtual GD_proxy_base* get_gd_proxy() override;
private:
	ComposeDoc* compose_doc_;
	GD_proxy_base* gd_proxy_;
};

class LxBorderRender : public LxRender
{
public:
	LxBorderRender() = delete;
	LxBorderRender(const LxBorderRender&) = delete;
	LxBorderRender(LxRender* base_render);
	virtual ~LxBorderRender();
public:
	virtual void create_caret(int height, int width) override;
	virtual void show_caret(LxCursor* cursor) override;
	virtual void hide_caret() override;
	virtual void DrawDocument(CDC* pDC) override;
	virtual ComposeDoc* get_compose_doc() override;
	virtual GD_proxy_base* get_gd_proxy() override;
private:
	void DrawBorder(CDC* pDC);
	void DrawPageSpace(CDC* pDC, int bottom_pos, int pages_space);
private:
	LxRender* base_render_;
};

#endif